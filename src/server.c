/*
 * Badger - A fear-less, robust and portable systems management solution.
 *
 * Copyright (c) 2010 Hal Brodigan (postmodern.mod3 at gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

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

	new_server->mode = badger_mode_none;
	new_server->uri = NULL;

	new_server->running = 0;
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
	if (service->init_func)
	{
		if (service->init_func() != BADGER_SUCCESS)
		{
			badger_debug("badger_server_register: could not initialize the server %s\n",service->name);
			return BADGER_ERROR;
		}
	}

	return slist_add(server->services,service->name,(badger_server_t *)service);
}

void badger_server_encoder(badger_server_t *server,badger_encoder_func encoder,void *data)
{
	server->encoder_func = encoder;
	server->encoder_data = data;
}

void badger_server_decoder(badger_server_t *server,badger_decoder_func decoder,void *data)
{
	server->decoder_func = decoder;
	server->decoder_data = data;
}

int badger_server_open(badger_server_t *server,badger_mode mode,const char *uri)
{
	server->mode = mode;

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

	// Create a ZMQ_P2P socket to receive requests and send replies
	if (!(server->zmq_socket = zmq_socket(server->zmq_context,ZMQ_P2P)))
	{
		goto cleanup_context;
	}

	switch (server->mode)
	{
		case badger_mode_connect:
			if (zmq_connect(server->zmq_socket,server->uri) == -1)
			{
				goto cleanup_socket;
			}
			break;
		case badger_mode_listen:
			if (zmq_bind(server->zmq_socket,server->uri) == -1)
			{
				goto cleanup_socket;
			}
			break;
		default:
			goto cleanup_socket;
	}

	server->running = 1;
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

void badger_server_loop(badger_server_t *server)
{
	while (server->running)
	{
		badger_server_pull(server);
	}
}

void badger_server_break(badger_server_t *server)
{
	server->running = 0;
}

int badger_server_close(badger_server_t *server)
{
	if (server->zmq_context)
	{
		if (zmq_term(server->zmq_context) == -1)
		{
			return -1;
		}

		server->zmq_socket = NULL;
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
