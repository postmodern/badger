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

#ifndef _PRIVATE_BADGER_RESPONSE_H_
#define _PRIVATE_BADGER_RESPONSE_H_

#include <badger/packet.h>
#include <badger/request.h>
#include <badger/response.h>

struct badger_server;

#include <msgpack/sbuffer.h>
#include <msgpack/pack.h>

struct badger_response
{
	badger_request_id id;
	badger_response_type type;

	struct msgpack_sbuffer buffer;
	struct msgpack_packer packer;
};
typedef struct badger_response badger_response_t;

void badger_response_init(badger_response_t *response,badger_request_id id,badger_response_type type);
void badger_response_clear(badger_response_t *response);
void badger_response_reset(badger_response_t *response,badger_response_type type);

#endif
