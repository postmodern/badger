#include <badger/packet.h>
#include "private/util.h"
#include "private/server.h"

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

	if (zmq_msg_size(&request) < BADGER_PACKET_MINSIZE)
	{
		// short packet
		goto cleanup_zmq_msg;
	}

	const void *packet = zmq_msg_data(&request);
	size_t packet_size = zmq_msg_size(&request);

	const void *packed_payload = (packet + sizeof(crc32_t));
	size_t payload_size = (packet_size - sizeof(crc32_t));

	crc32_t claimed_checksum = ntohl(*(crc32_t *)packet);
	crc32_t actual_checksum = badger_crc32(packed_payload,payload_size);

	if (claimed_checksum != actual_checksum)
	{
		// corrupted packet
		goto cleanup_zmq_msg;
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

			badger_server_process(server,&payload);
		case MSGPACK_UNPACK_CONTINUE:
			msgpack_zone_free(zone);
			break;
		case MSGPACK_UNPACK_PARSE_ERROR:
			return -1;
	}

	return 0;

cleanup_msgpack_object:
	msgpack_zone_free(zone);
cleanup_zmq_msg:
	zmq_msg_close(&request);
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
	return -1;
}
