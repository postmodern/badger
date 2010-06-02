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

#include "private/ffi/function.h"
#include "private/util.h"
#include <badger/errno.h>

#include <malloc.h>
#include <string.h>

ffi_function_t * ffi_function_create(const char *name,void *ptr,const ffi_type *ret_type,size_t argc,const ffi_type **arg_types)
{
	char *new_name;

	if (!(new_name = strdup(name)))
	{
		goto cleanup;
	}

	const ffi_type **new_arg_types;

	if (!(new_arg_types = malloc(sizeof(ffi_type *))))
	{
		goto cleanup_name;
	}

	memcpy(new_arg_types,arg_types,sizeof(ffi_type *) * argc);

	ffi_function_t *new_function;

	if (!(new_function = malloc(sizeof(ffi_function_t))))
	{
		goto cleanup_arg_types;
	}

	new_function->name = new_name;
	new_function->ptr = ptr;

	new_function->ret_type = ret_type;
	new_function->argc = argc;
	new_function->arg_types = new_arg_types;

	ffi_prep_cif(&(new_function->cif),FFI_DEFAULT_ABI,new_function->argc,(ffi_type *)new_function->ret_type,(ffi_type **)new_function->arg_types);
	return new_function;

cleanup_arg_types:
	free(new_arg_types);
cleanup_name:
	free(new_name);
cleanup:
	return NULL;
}

void ffi_function_call(const ffi_function_t *function,size_t argc,const ffi_value_t *args,ffi_value_t *ret)
{
	void *arg_ptrs[argc];
	unsigned int i;

	for (i=0;i<argc;i++)
	{
		arg_ptrs[i] = (void *)(args+i);
	}

	ffi_cif *cif = (ffi_cif *)&(function->cif);

	ffi_call(cif,FFI_FN(function->ptr),ret,arg_ptrs);
}

void ffi_function_destroy(ffi_function_t *function)
{
	free(function->name);

	// zero the arg types before freeing them
	memset(function->arg_types,0,sizeof(ffi_type *) * function->argc);
	free(function->arg_types);

	// zero the function before freeing it
	memset(function,0,sizeof(ffi_function_t));
	free(function);
}

int badger_ffi_function_valid(const ffi_function_t *function,int argc,const badger_data_t *args)
{
	if (argc != function->argc)
	{
		return BADGER_ERRNO_ARGC;
	}

	unsigned int i;

	for (i=0;i<function->argc;i++)
	{
		const badger_data_t *arg = args+i;

		switch (function->arg_types[i]->type)
		{
			case FFI_TYPE_INT:
			case FFI_TYPE_UINT8:
			case FFI_TYPE_UINT16:
			case FFI_TYPE_UINT32:
			case FFI_TYPE_UINT64:
			case FFI_TYPE_SINT8:
			case FFI_TYPE_SINT16:
			case FFI_TYPE_SINT32:
			case FFI_TYPE_SINT64:
				switch (badger_type(arg))
				{
					case badger_data_int:
					case badger_data_uint:
						break;
					default:
						// invalid given argument type
						badger_debug("incompatible argument type for FFI int");
						return BADGER_ERRNO_ARG_TYPE;
				}
				break;
			case FFI_TYPE_FLOAT:
			case FFI_TYPE_DOUBLE:
			case FFI_TYPE_LONGDOUBLE:
				switch (badger_type(arg))
				{
					case badger_data_float:
						break;
					default:
						badger_debug("incompatible argument type for FFI float");
						return BADGER_ERRNO_ARG_TYPE;
				}
				break;
			case FFI_TYPE_POINTER:
				switch (badger_type(arg))
				{
					case badger_data_uint:
					case badger_data_raw:
						break;
					default:
						badger_debug("incompatible argument type for FFI pointer");
						return BADGER_ERRNO_ARG_TYPE;
				}
				break;
			default:
				badger_debug("badger_ffi_invoke: unknown FFI type (%u)\n",function->arg_types[i]->type);
				return BADGER_ERROR;
		}
	}

	return 0;
}
