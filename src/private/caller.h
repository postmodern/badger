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

#ifndef _PRIVATE_BADGER_CALLER_H_
#define _PRIVATE_BADGER_CALLER_H_

#include <badger/caller.h>
#include "private/response.h"

struct badger_caller
{
	struct badger_server *server;
	unsigned returned:1;
	unsigned multi_return:1;
	unsigned yielded:1;

	badger_response_t ret;
	badger_response_t yield;
};

void badger_caller_init(badger_caller_t *caller,badger_request_id id,struct badger_server *server);
void badger_caller_returned(badger_caller_t *caller);
int badger_caller_return(badger_caller_t *caller);
void badger_caller_fini(badger_caller_t *caller);

#endif
