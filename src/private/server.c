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
#include <alloca.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <msgpack/sbuffer.h>
#include <msgpack/pack.h>
#include <msgpack/unpack.h>

int badger_server_pull(badger_server_t *server)
{
	zmq_msg_t request;

	if (zmq_msg_init(&request) == -1)
	{
		goto cleanup;
	}

	if (zmq_recv(server->zmq_socket,&request,0) == -1)
	{
		goto cleanup_zmq_msg;
	}

	// decode the packet
	badger_server_decode(server,&request);

	// close the message
	zmq_msg_close(&request);
	return 0;

cleanup_zmq_msg:
	zmq_msg_close(&request);
cleanup:
	return -1;
}

int badger_server_decode(badger_server_t *server,zmq_msg_t *request)
{
	size_t packet_size = zmq_msg_size(request);

	if (packet_size < BADGER_PACKET_MINSIZE)
	{
		// ignore short packets
		return -1;
	}

	if (server->decoder_func)
	{
		int ret;
		unsigned char packet[packet_size];

		// decode the packet
		server->decoder_func(packet,zmq_msg_data(request),packet_size,server->decoder_data);

		// unpack the payload
		ret = badger_server_unpack(server,packet,packet_size);

		// zero the decoded packet
		memset(packet,0,packet_size);
		return ret;
	}

	return badger_server_unpack(server,zmq_msg_data(request),packet_size);
}

int badger_server_unpack(badger_server_t *server,const unsigned char *packet,size_t packet_size)
{
	if (!badger_packet_valid(packet,packet_size))
	{
		// ignore invalid packets
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
				goto cleanup_msgpack_object;
			}

			// process the payload
			badger_server_dispatch(server,&payload);

		case MSGPACK_UNPACK_CONTINUE:
			msgpack_zone_free(zone);
			break;
		case MSGPACK_UNPACK_PARSE_ERROR:
			// invalid payload
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
	*((crc32_t *)(packed_payload+1)) = badger_crc32(payload,payload_size);

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

	if (zmq_msg_init_data(&response,(void *)packet,packet_size,NULL,NULL) != 0)
	{
		goto cleanup;
	}

	if (zmq_send(server->zmq_socket,&response,0) != 0)
	{
		goto cleanup_zmq_msg;
	}

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
		goto ignore;
	}

	const msgpack_object *fields = payload->via.array.ptr;

	if (fields[0].type != MSGPACK_OBJECT_POSITIVE_INTEGER)
	{
		// the ID field must be a positive integer
		goto ignore;
	}

	if (!fields[0].via.u64)
	{
		// the ID field must not be empty
		goto ignore;
	}

	if (fields[1].type != MSGPACK_OBJECT_POSITIVE_INTEGER)
	{
		// the type field must be a positive integer
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
			goto ignore;
	}

	badger_request_id id = fields[0].via.u64;

	switch (fields[1].via.u64)
	{
		case BADGER_REQUEST_PING:
			return badger_server_pong(server,id,fields);
		case BADGER_REQUEST_CALL:
			if (length < 5)
			{
				// function call payloads must have atleast five fields
				goto ignore;
			}

			// call the function
			return badger_server_call(server,id,fields+2);
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

int badger_server_call(badger_server_t *server,badger_request_id id,const msgpack_object *fields)
{
	if (fields[0].type != MSGPACK_OBJECT_RAW)
	{
		// the service field must be a string
		return -1;
	}

	if (!fields[0].via.raw.size)
	{
		// the service field must have atleast one character
		return -1;
	}

	if (!fields[0].via.raw.ptr[0])
	{
		// the service field must not be empty
		return -1;
	}

	if (fields[1].type != MSGPACK_OBJECT_RAW)
	{
		// the name field must be a string
		return -1;
	}

	if (!fields[1].via.raw.size)
	{
		// the name field must have atleast one character
		return -1;
	}

	if (!fields[1].via.raw.ptr[0])
	{
		// the name field must not be empty
		return -1;
	}

	if (fields[2].type != MSGPACK_OBJECT_ARRAY)
	{
		// the arguments field must be an Array
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
	badger_func_call(func,argc,args,&caller);
	badger_caller_fini(&caller);
	return 0;
}
