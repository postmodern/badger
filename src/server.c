#include <badger/server.h>

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
	zmq_msg_t request;
	zmq_msg_t response;

	while (1)
	{
		if (zmq_msg_init(&request) != 0)
		{
			return -1;
		}

		if (zmq_recv(server->zmq_socket,&request,0) != 0)
		{
			return -1;
		}

		// TODO: validate, deserialize and process the received
		// message
		zmq_msg_close(&request);

		// TODO: serialize response

		if (zmq_msg_init_size(&response,0) != 0)
		{
			return -1;
		}

		if (zmq_send(server->zmq_socket,&response,0) != 0)
		{
			return -1;
		}

		zmq_msg_close(&response);
	}

	return 0;
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
	free(server);
}
