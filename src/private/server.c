#include <badger/packet.h>
#include <badger/data.h>
#include <badger/func.h>
#include <badger/errno.h>
#include "private/util.h"
#include "private/packet.h"
#include "private/request.h"
#include "private/response.h"
#include "private/caller.h"
#include "private/server.h"

#include <time.h>
#include <msgpack/pack.h>
#include <msgpack/unpack.h>

int badger_server_pull(badger_server_t *server)
{
	zmq_msg_t request;

	if (zmq_msg_init(&request) == -1)
	{
		badger_debug("badger_server_pull: zmq_msg_init failed\n");
		goto cleanup;
	}

	if (zmq_recv(server->zmq_socket,&request,0) == -1)
	{
		badger_debug("badger_server_pull: zmq_recv failed\n");
		goto cleanup_zmq_msg;
	}

	// decode the packet
	badger_server_decode(server,zmq_msg_data(&request),zmq_msg_size(&request));

	// close the message
	zmq_msg_close(&request);
	return 0;

cleanup_zmq_msg:
	zmq_msg_close(&request);
cleanup:
	return -1;
}

int badger_server_decode(badger_server_t *server,const unsigned char *packet,size_t packet_size)
{
	if (packet_size < BADGER_PACKET_MINSIZE)
	{
		// ignore short packets
		badger_debug("badger_server_decode: the packet length (%lu) was less than BADGER_PACKET_MINSIZE (%u)\n",packet_size,BADGER_PACKET_MINSIZE);
		return -1;
	}

	if (server->decoder_func)
	{
		int ret;
		unsigned char decoded_packet[packet_size];

		// decode the packet
		server->decoder_func(decoded_packet,packet,packet_size,server->decoder_data);

		// unpack the payload
		ret = badger_server_unpack(server,decoded_packet,packet_size);

		// zero the decoded packet
		memset(decoded_packet,0,packet_size);
		return ret;
	}

	return badger_server_unpack(server,packet,packet_size);
}

int badger_server_unpack(badger_server_t *server,const unsigned char *packet,size_t packet_size)
{
	if (!badger_packet_valid(packet,packet_size))
	{
		// ignore invalid packets
		badger_debug("badger_server_unpack: packet was not valid\n");
		goto cleanup;
	}

	const void *packed_payload = badger_packet_payload(packet);
	size_t payload_size = (packet_size - BADGER_PACKET_HDRSIZE);

	msgpack_zone *zone = msgpack_zone_new(MSGPACK_ZONE_CHUNK_SIZE);
	size_t offset = 0;

	msgpack_object payload;

	switch (msgpack_unpack(packed_payload,payload_size,&offset,zone,&payload))
	{
		case MSGPACK_UNPACK_SUCCESS:
		case MSGPACK_UNPACK_EXTRA_BYTES:
			if (payload.type != MSGPACK_OBJECT_ARRAY)
			{
				// invalid payload
				badger_debug("badger_server_unpack: unpacked MsgPack object was not an Array\n");
				goto cleanup_msgpack_object;
			}

			// process the payload
			badger_server_dispatch(server,&payload);

		case MSGPACK_UNPACK_CONTINUE:
			msgpack_zone_free(zone);
			break;
		case MSGPACK_UNPACK_PARSE_ERROR:
			// invalid payload
			badger_debug("badger_server_unpack: parse error encountered in msgpack_unpack\n");
			goto cleanup_msgpack_object;
	}

	return 0;

cleanup_msgpack_object:
	msgpack_zone_free(zone);
cleanup:
	return -1;
}

int badger_server_pack(const badger_server_t *server,const char *payload,size_t payload_size)
{
	size_t packed_payload_size = payload_size + BADGER_PACKET_HDRSIZE;
	unsigned char packed_payload[packed_payload_size];
	int ret;

	// add the Badger Protocol version
	packed_payload[0] = BADGER_PROTOCOL_VERSION;

	// add the CRC32 checksum
	*((crc32_t *)(packed_payload+1)) = htonl(badger_crc32(payload,payload_size));

	memcpy(packed_payload+BADGER_PACKET_HDRSIZE,payload,payload_size);

	// encode and push the packet
	ret = badger_server_encode(server,packed_payload,packed_payload_size);

	// zero the packed payload
	memset(packed_payload,0,packed_payload_size);
	return ret;
}

int badger_server_encode(const badger_server_t *server,const unsigned char *packet,size_t packet_size)
{
	if (server->encoder_func)
	{
		int ret;
		unsigned char encoded_packet[packet_size];

		// encode the packet
		server->encoder_func(encoded_packet,packet,packet_size,server->encoder_data);

		// push the encoded packet
		ret = badger_server_push(server,encoded_packet,packet_size);

		// zero the encoded packet
		memset(encoded_packet,0,packet_size);
		return ret;
	}

	// push the packet
	return badger_server_push(server,packet,packet_size);
}

int badger_server_push(const badger_server_t *server,const unsigned char *packet,size_t packet_size)
{
	zmq_msg_t response;

	if (zmq_msg_init_size(&response,packet_size) != 0)
	{
		badger_debug("badger_server_push: zmq_msg_init_data failed\n");
		goto cleanup;
	}

	// copy in the packet
	memcpy(zmq_msg_data(&response),packet,packet_size);

	if (zmq_send(server->zmq_socket,&response,0) != 0)
	{
		badger_debug("badger_server_push: zmq_send failed\n");
		goto cleanup_zmq_msg;
	}

	// zero the zmq msg before freeing it
	memset(zmq_msg_data(&response),0,packet_size);

	zmq_msg_close(&response);
	return 0;

cleanup_zmq_msg:
	zmq_msg_close(&response);
cleanup:
	return -1;
}

int badger_server_dispatch(badger_server_t *server,const msgpack_object *payload)
{
	const uint32_t length = payload->via.array.size;

	if (length < 2)
	{
		// there must be a minimum of two fields in the payload
		badger_debug("badger_server_dispatch: packet payload has less than two fields\n");
		goto ignore;
	}

	const msgpack_object *fields = payload->via.array.ptr;

	if (fields[0].type != MSGPACK_OBJECT_POSITIVE_INTEGER)
	{
		// the ID field must be a positive integer
		badger_debug("badger_server_dispatch: request ID field of the packet payload was not a positive integer\n");
		goto ignore;
	}

	if (fields[0].via.u64 < BADGER_REQUEST_ID_MIN)
	{
		// the ID field must not be less than 1
		badger_debug("badger_server_dispatch: request ID of the packet payload was less than BADGER_REQUEST_ID_MIN\n");
		goto ignore;
	}

	if (fields[0].via.u64 > BADGER_REQUEST_ID_MAX)
	{
		// the ID field must not overflow badger_request_id
		badger_debug("badger_server_dispatch: request ID of the packet payload was greater than BADGER_REQUEST_ID_MAX\n");
		goto ignore;
	}

	if (fields[1].type != MSGPACK_OBJECT_POSITIVE_INTEGER)
	{
		// the type field must be a positive integer
		badger_debug("badger_server_dispatch: request type field of the packet payload was not a positive integer\n");
		goto ignore;
	}

	switch (fields[1].via.u64)
	{
		// valid payload types
		case BADGER_REQUEST_PING:
		case BADGER_REQUEST_CALL:
			break;
		default:
			// ignore unknown payload types
			badger_debug("badger_server_dispatch: request type of the packet payload was not recognized\n");
			goto ignore;
	}

	badger_request_id id = fields[0].via.u64;
	const msgpack_object *extra_fields = fields + 2;

	switch (fields[1].via.u64)
	{
		case BADGER_REQUEST_PING:
			return badger_server_pong(server,id,NULL);
		case BADGER_REQUEST_SERVICES:
			return badger_server_services(server,id,NULL);
		case BADGER_REQUEST_FUNCTIONS:
			return badger_server_functions(server,id,extra_fields);
		case BADGER_REQUEST_CALL:
			if (length < 5)
			{
				// function call payloads must have atleast five fields
				badger_debug("badger_server_dispatch: the packet payload had less than five fields needed for a CALL request\n");
				goto ignore;
			}

			// call the function
			return badger_server_call(server,id,extra_fields);
		default:
			// ignore other packet types
			goto ignore;
	}

	return 0;

ignore:
	return -1;
}

int badger_server_pong(badger_server_t *server,badger_request_id id,const msgpack_object *fields)
{
	badger_response_t response;
	int ret;

	// initialize the pong response
	badger_response_init(&response,id,BADGER_RESPONSE_PONG);

	// add the current time to the pong response
	msgpack_pack_unsigned_int(&(response.packer),time(NULL));

	// pack, encode and push the response
	ret = badger_server_pack(server,response.buffer.data,response.buffer.size);

	badger_response_clear(&response);
	return ret;
}

int badger_server_services(badger_server_t *server,badger_request_id id,const msgpack_object *fields)
{
	badger_response_t response;

	// initialize the services response
	badger_response_init(&response,id,BADGER_RESPONSE_SERVICES);

	msgpack_pack_array(&(response.packer),slist_length(server->services));

	const slist_node_t *next_node = server->services->head;

	while (next_node)
	{
		const badger_service_t *service = (badger_service_t *)(next_node->data);
		size_t name_length = strlen(service->name) + 1;

		msgpack_pack_raw(&(response.packer),name_length);
		msgpack_pack_raw_body(&(response.packer),service->name,name_length);

		next_node = next_node->next;
	}

	int ret;

	// pack, encode and push the response
	ret = badger_server_pack(server,response.buffer.data,response.buffer.size);

	badger_response_clear(&response);
	return ret;
}

int badger_server_functions(badger_server_t *server,badger_request_id id,const msgpack_object *fields)
{
	if (fields[0].type != MSGPACK_OBJECT_RAW)
	{
		// the service field must be a String
		badger_debug("badger_server_functions: the service field of the FUNCTIONS request was not a String\n");
		return -1;
	}

	if (!fields[0].via.raw.size)
	{
		// the service field must have atleast one character
		badger_debug("badger_server_functions: the service field of the FUNCTIONS request had an empty length\n");
		return -1;
	}

	if (!fields[0].via.raw.ptr[0])
	{
		// the service field must not be empty
		badger_debug("badger_server_functions: the service field of the FUNCTIONS request was empty\n");
		return -1;
	}

	size_t service_length = fields[0].via.raw.size;
	char service_name[service_length + 1];

	memcpy(service_name,fields[0].via.raw.ptr,service_length);
	service_name[service_length] = '\0';

	const badger_service_t *service;

	if (!(service = slist_search(server->services,service_name)))
	{
		// service not found
		badger_debug("badger_server_functions: could not find the requested service (%s)\n",service_name);
		return -1;
	}

	badger_response_t response;

	// initialize the functions response
	badger_response_init(&response,id,BADGER_RESPONSE_FUNCTIONS);

	unsigned int length = 0;
	unsigned int i = 0;

	while (service->funcs[i].name)
	{
		++length;
		++i;
	}

	msgpack_pack_array(&(response.packer),length);

	for (i=0;i<length;i++)
	{
		size_t name_length = strlen(service->funcs[i].name) + 1;

		msgpack_pack_raw(&(response.packer),name_length);
		msgpack_pack_raw_body(&(response.packer),service->funcs[i].name,name_length);

		++i;
	}

	int ret;

	// pack, encode and push the response
	ret = badger_server_pack(server,response.buffer.data,response.buffer.size);

	badger_response_clear(&response);
	return 0;
}

int badger_server_call(badger_server_t *server,badger_request_id id,const msgpack_object *fields)
{
	if (fields[0].type != MSGPACK_OBJECT_RAW)
	{
		// the service field must be a String
		badger_debug("badger_server_call: the service field of the CALL request was not a String\n");
		return -1;
	}

	if (!fields[0].via.raw.size)
	{
		// the service field must have atleast one character
		badger_debug("badger_server_call: the service field of the CALL request had an empty length\n");
		return -1;
	}

	if (!fields[0].via.raw.ptr[0])
	{
		// the service field must not be empty
		badger_debug("badger_server_call: the service field of the CALL request was empty\n");
		return -1;
	}

	if (fields[1].type != MSGPACK_OBJECT_RAW)
	{
		// the name field must be a String
		badger_debug("badger_server_call: the name field of the CALL request was not a String\n");
		return -1;
	}

	if (!fields[1].via.raw.size)
	{
		// the name field must have atleast one character
		badger_debug("badger_server_call: the name field of the CALL request had an empty length\n");
		return -1;
	}

	if (!fields[1].via.raw.ptr[0])
	{
		// the name field must not be empty
		badger_debug("badger_server_call: the name field of the CALL request was empty\n");
		return -1;
	}

	if (fields[2].type != MSGPACK_OBJECT_ARRAY)
	{
		// the arguments field must be an Array
		badger_debug("badger_server_call: the arguments field of the CALL request was not an Array\n");
		return -1;
	}

	int argc = (int)fields[2].via.array.size;
	const badger_data_t *args = fields[2].via.array.ptr;

	size_t service_length = fields[0].via.raw.size;
	char service_name[service_length + 1];

	memcpy(service_name,fields[0].via.raw.ptr,service_length);
	service_name[service_length] = '\0';

	const badger_service_t *service;

	if (!(service = slist_search(server->services,service_name)))
	{
		// service not found
		badger_debug("badger_server_call: could not find the requested service (%s)\n",service_name);
		return -1;
	}

	size_t name_length = fields[1].via.raw.size;
	char name[name_length + 1];

	memcpy(name,fields[1].via.raw.ptr,name_length);
	name[name_length] = '\0';

	const badger_func_t *func;

	if (!(func = badger_service_search(service,name)))
	{
		// function not found
		badger_debug("badger_server_call: could not find the requested function (%s) from the service (%s)\n",name,service->name);
		return -1;
	}

	switch (badger_func_valid(func,argc,args))
	{
		case BADGER_ERRNO_ARGC:
			// wrong number of arguments
			return -1;
		case BADGER_ERRNO_ARG_TYPE:
			// wrong argument type
			return -1;
	}

	badger_caller_t caller;

	badger_caller_init(&caller,id,server);

	// call the function
	badger_func_call(func,argc,args,&caller);

	// send back the return data
	badger_server_pack(server,caller.ret.buffer.data,caller.ret.buffer.size);

	badger_caller_fini(&caller);
	return 0;
}
