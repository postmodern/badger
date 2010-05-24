#include "private/server.h"
#include "private/util.h"
#include <badger/server.h>
#include <badger/packet.h>

#include <malloc.h>
#include <string.h>

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

	new_server->encoder_func = NULL;
	new_server->encoder_data = NULL;

	new_server->decoder_func = NULL;
	new_server->decoder_data = NULL;

	if (!(new_server->services = slist_create(slist_compare_strings,NULL)))
	{
		goto cleanup_new_server;
	}

	return new_server;

cleanup_new_server:
	free(new_server);
cleanup:
	return NULL;
}

int badger_server_register(badger_server_t *server,const badger_service_t *service)
{
	return slist_add(server->services,service->name,(badger_server_t *)service);
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

void badger_server_listen(const badger_server_t *server)
{
	while (1)
	{
		badger_server_recv(server);
	}
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
	server->encoder_func = NULL;
	server->encoder_data = NULL;
	server->decoder_func = NULL;
	server->decoder_data = NULL;
	server->uri = NULL;
	server->zmq_context = NULL;
	server->zmq_socket = NULL;
	server->services = NULL;

	free(server);
}
