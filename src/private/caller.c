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

#include "private/caller.h"
#include "private/server.h"

void badger_caller_init(badger_caller_t *caller,badger_request_id id,struct badger_server *server)
{
	caller->server = server;
	caller->returned = 0;
	caller->multi_return = 0;
	caller->yielded = 0;

	badger_response_init(&(caller->ret),id,BADGER_RESPONSE_RETURN);
}

void badger_caller_returned(badger_caller_t *caller)
{
	if (caller->returned && !(caller->multi_return))
	{
		badger_response_reset(&(caller->ret),BADGER_RESPONSE_RETURN);
		caller->returned = 0;
	}
	else
	{
		caller->returned = 1;
	}
}

int badger_caller_return(badger_caller_t *caller)
{
	if (!(caller->returned))
	{
		badger_return_nil(caller);
	}

	// send back the return data
	return badger_server_pack(caller->server,caller->ret.buffer.data,caller->ret.buffer.size);
}

void badger_caller_fini(badger_caller_t *caller)
{
	badger_response_clear(&(caller->ret));

	if (caller->yielded)
	{
		badger_response_clear(&(caller->yield));
	}
}
