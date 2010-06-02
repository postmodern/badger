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

#include <badger/function.h>
#include <badger/errno.h>

int badger_function_valid(const badger_function_t *function,int argc,const badger_data_t *args)
{
	if (function->argc != -1)
	{
		if (argc != function->argc)
		{
			return BADGER_ERRNO_ARGC;
		}

		unsigned int i;

		for (i=0;i<argc;i++)
		{
			if (function->arg_types[i] != badger_data_any)
			{
				if (args[i].type != function->arg_types[i])
				{
					return BADGER_ERRNO_ARG_TYPE;
				}
			}
		}
	}

	return 0;
}

int badger_function_call(const badger_function_t *function,int argc,const badger_data_t *args,badger_caller_t *caller)
{
	return function->ptr(argc,args,caller);
}
