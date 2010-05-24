#include <badger/packet.h>
#include "private/util.h"
#include "private/server.h"

#include <alloca.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <msgpack/sbuffer.h>
#include <msgpack/pack.h>
#include <msgpack/unpack.h>

int badger_server_recv(const badger_server_t *server)
{
	zmq_msg_t request;

	if (zmq_msg_init(&request) != 0)
	{
		goto cleanup;
	}

	if (zmq_recv(server->zmq_socket,&request,0) != 0)
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

int badger_server_decode(const badger_server_t *server,zmq_msg_t *request)
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

int badger_server_unpack(const badger_server_t *server,const unsigned char *packet,size_t packet_size)
{
	const void *packed_payload = (packet + sizeof(crc32_t));
	size_t payload_size = (packet_size - sizeof(crc32_t));

	crc32_t claimed_checksum = ntohl(*(crc32_t *)packet);
	crc32_t actual_checksum = badger_crc32(packed_payload,payload_size);

	if (claimed_checksum != actual_checksum)
	{
		// ignore corrupted packets
		goto cleanup;
	}

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
			badger_server_process(server,&payload);

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

int badger_server_send(const badger_server_t *server,const msgpack_object *payload)
{
	msgpack_sbuffer sbuf;
	msgpack_packer packer;

	msgpack_sbuffer_init(&sbuf);
	msgpack_packer_init(&packer,&sbuf,msgpack_sbuffer_write);

	msgpack_pack_object(&packer,*payload);

	size_t packet_size = sizeof(crc32_t) + sbuf.size;
	zmq_msg_t response;

	if (zmq_msg_init_size(&response,packet_size) != 0)
	{
		goto cleanup;
	}

	void *packet = zmq_msg_data(&response);
	crc32_t payload_checksum = htonl(badger_crc32(sbuf.data,sbuf.size));

	// set the crc32 payload checksum
	*((crc32_t *)packet) = payload_checksum;

	// copy the packed msgpack object into the packet
	memcpy(packet+sizeof(crc32_t),sbuf.data,sbuf.size);

	// zero and destroy the packed msgpack object
	memset(sbuf.data,0,sbuf.size);
	msgpack_sbuffer_destroy(&sbuf);

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

int badger_server_process(const badger_server_t *server,const msgpack_object *payload)
{
	const uint32_t length = payload->via.array.size;

	if (length < 2)
	{
		// there must be a minimum of two fields in the payload
		return -1;
	}

	const msgpack_object *fields = payload->via.array.ptr;

	if (fields[0].type != MSGPACK_OBJECT_POSITIVE_INTEGER)
	{
		// the type field must be a positive integer
		return -1;
	}

	if (fields[1].type != MSGPACK_OBJECT_POSITIVE_INTEGER)
	{
		// the timestamp field must be a positive integer
		return -1;
	}

	if (!fields[1].via.u64)
	{
		// the timestamp field must not be empty
		return -1;
	}

	switch (fields[0].via.u64)
	{
		case BADGER_PACKET_PING:
			// send a pong
			break;
		case BADGER_PACKET_CALL:
			if (length < 5)
			{
				// function call payloads must have atleast five fields
				return -1;
			}

			if (fields[2].type != MSGPACK_OBJECT_RAW)
			{
				// the service field must be a string
				return -1;
			}

			if (!fields[2].via.raw.size)
			{
				// the service field must have atleast one character
				return -1;
			}

			if (!fields[2].via.raw.ptr[0])
			{
				// the service field must not be empty
				return -1;
			}

			if (fields[3].type != MSGPACK_OBJECT_RAW)
			{
				// the name field must be a string
				return -1;
			}

			if (!fields[3].via.raw.size)
			{
				// the name field must have atleast one character
				return -1;
			}

			if (!fields[3].via.raw.ptr[0])
			{
				// the name field must not be empty
				return -1;
			}

			if (fields[4].type != MSGPACK_OBJECT_ARRAY)
			{
				// the arguments field must be an Array
				return -1;
			}

			// call the function
			break;
		default:
			// ignore other packet types
			return -1;
	}

	return 0;
}
