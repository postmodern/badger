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

#ifndef _PRIVATE_FFI_FUNCTION_H_
#define _PRIVATE_FFI_FUNCTION_H_

#include "private/ffi/value.h"
#include <badger/data.h>
#include <badger/caller.h>

#include <ffi.h>

struct ffi_function
{
	char *name;
	void *ptr;

	const ffi_type *ret_type;

	size_t argc;
	const ffi_type **arg_types;

	ffi_cif cif;
};
typedef struct ffi_function ffi_function_t;

ffi_function_t * ffi_function_create(const char *name,void *ptr,const ffi_type *ret_type,size_t argc,const ffi_type **arg_types);
void ffi_function_call(const ffi_function_t *function,size_t argc,const ffi_value_t *args,ffi_value_t *ret);
void ffi_function_destroy(ffi_function_t *function);

int badger_ffi_function_valid(const ffi_function_t *function,int argc,const badger_data_t *args);

#endif
