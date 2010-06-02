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

#ifndef _PRIVATE_BADGER_SERVER_H_
#define _PRIVATE_BADGER_SERVER_H_

#include <badger/server.h>
#include <badger/request.h>

int badger_server_pull(badger_server_t *server);
int badger_server_decode(badger_server_t *server,const unsigned char *packet,size_t packet_size);
int badger_server_unpack(badger_server_t *server,const unsigned char *packed_payload,size_t packet_size);

int badger_server_pack(const badger_server_t *server,const char *payload,size_t payload_size);
int badger_server_encode(const badger_server_t *server,const unsigned char *packet,size_t packet_size);
int badger_server_push(const badger_server_t *server,const unsigned char *payload,size_t packet_size);

int badger_server_dispatch(badger_server_t *server,const msgpack_object *payload);

const badger_service_t * badger_server_get_service(badger_server_t *server,badger_request_id id,const msgpack_object *fields);
const badger_function_t * badger_server_get_function(badger_server_t *server,badger_request_id id,const msgpack_object *fields);

int badger_server_pong(badger_server_t *server,badger_request_id id,const msgpack_object *fields);
int badger_server_services(badger_server_t *server,badger_request_id id,const msgpack_object *fields);
int badger_server_functions(badger_server_t *server,badger_request_id id,const msgpack_object *fields);
int badger_server_prototype(badger_server_t *server,badger_request_id id,const msgpack_object *fields);
int badger_server_call(badger_server_t *server,badger_request_id id,const msgpack_object *fields);

#endif
