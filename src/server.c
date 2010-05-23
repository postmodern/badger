#include <badger/server.h>
#include <badger/packet.h>
#include <badger/util.h>

#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <msgpack/sbuffer.h>
#include <msgpack/pack.h>
#include <msgpack/unpack.h>

badger_server_t * badger_server_create()
{
	badger_server_t *new_server;

	if (!(new_server = malloc(sizeof(badger_server_t))))
	{
		goto cleanup;
	}

	new_server->uri = NULL;
	new_server->zmq_context = NULL;
	new_server->zmq_socket = NULL;

	if (!(new_server->services = slist_create(slist_compare_strings,(slist_destroy_func)badger_service_destroy)))
	{
		goto cleanup_new_server;
	}

cleanup_new_server:
	free(new_server);
cleanup:
	return NULL;
}

int badger_server_register(badger_server_t *server,const char *service,const char *name,badger_func_ptr ptr,int argc,msgpack_object_type *arg_types)
{
	badger_service_t *new_service;

	if (!(new_service = slist_search(server->services,service)))
	{
		if (!(new_service = badger_service_create(service)))
		{
			goto cleanup;
		}

		if (slist_add(server->services,new_service->name,new_service))
		{
			goto cleanup_new_service;
		}

		if (badger_service_register(new_service,name,ptr,argc,arg_types))
		{
			goto cleanup_new_service;
		}

		return 0;
	}

	return badger_service_register(new_service,name,ptr,argc,arg_types);

cleanup_new_service:
	badger_service_destroy(new_service);
cleanup:
	return -1;
}

int badger_server_open(badger_server_t *server,const char *uri)
{
	if (server->uri)
	{
		free(server->uri);
	}

	if (!(server->uri = strdup(uri)))
	{
		// malloc failure
		goto cleanup;
	}

	// Initialise 0MQ context, requesting a single application thread
	// and a single I/O thread
	if (!(server->zmq_context = zmq_init(1, 1, 0)))
	{
		goto cleanup;
	}

	// Create a ZMQ_REP socket to receive requests and send replies
	if (!(server->zmq_socket = zmq_socket(server->zmq_context,ZMQ_REP)))
	{
		goto cleanup_context;
	}

	if (zmq_bind(server->zmq_socket,uri) != 0)
	{
		goto cleanup_socket;
	}

	return 0;

cleanup_socket:
	zmq_close(server->zmq_socket);
	server->zmq_socket = NULL;

cleanup_context:
	zmq_term(server->zmq_context);
	server->zmq_context = NULL;
cleanup:
	return -1;
}

int badger_server_listen(const badger_server_t *server)
{
	while (1)
	{
		badger_server_recv(server);
	}

	return 0;
}

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

int badger_server_close(badger_server_t *server)
{
	if (server->zmq_socket)
	{
		if (zmq_close(server->zmq_socket) != 0)
		{
			return -1;
		}

		server->zmq_socket = NULL;
	}

	if (server->zmq_context)
	{
		if (zmq_term(server->zmq_context) != 0)
		{
			return -1;
		}

		server->zmq_context = NULL;
	}

	return 0;
}

void badger_server_destroy(badger_server_t *server)
{
	if (server->uri)
	{
		free(server->uri);
	}

	slist_destroy(server->services);

	// zero the server fields before freeing it
	server->uri = NULL;
	server->zmq_context = NULL;
	server->zmq_socket = NULL;
	server->services = NULL;

	free(server);
}
