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

#ifndef _BADGER_FUNCTION_H_
#define _BADGER_FUNCTION_H_

#include <badger/data.h>
#include <badger/caller.h>

#include <sys/types.h>

typedef int (*badger_function_ptr)(int argc,const badger_data_t *args,badger_caller_t *caller);

struct badger_function
{
	const char *name;

	badger_function_ptr ptr;

	badger_data_type ret_type;
	ssize_t argc;
	const badger_data_type arg_types[];
};
typedef struct badger_function badger_function_t;

extern int badger_function_valid(const badger_function_t *function,int argc,const badger_data_t *args);
extern int badger_function_call(const badger_function_t *function,int argc,const badger_data_t *args,badger_caller_t *caller);

#endif
