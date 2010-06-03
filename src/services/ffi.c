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

#include <badger/services/ffi.h>
#include <badger/slist.h>

#include "private/ffi/types.h"
#include "private/ffi/value.h"
#include "private/ffi/library.h"
#include "private/util.h"

const badger_service_t badger_ffi_service = {
	"ffi",
	badger_ffi_init,
	{
		&badger_ffi_open_func,
		&badger_ffi_libraries_func,
		&badger_ffi_attach_function_func,
		&badger_ffi_invoke_func,
		&badger_ffi_malloc_func,
		&badger_ffi_free_func,
		&badger_ffi_read_func,
		&badger_ffi_write_func,
		&badger_ffi_close_func,
		NULL
	}
};

slist_t *ffi_libraries = NULL;

int badger_ffi_init()
{
	ffi_libraries = slist_create(slist_compare_strings,(slist_destroy_func)ffi_library_close);
	return BADGER_SUCCESS;
}

const badger_function_t badger_ffi_open_func = {"open",badger_ffi_open,badger_data_boolean,1,{badger_data_string}};

int badger_ffi_open(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	size_t name_length = badger_string_length(args);
	char name[name_length + 1];

	memcpy(name,badger_string(args),name_length);
	name[name_length] = '\0';

	ffi_library_t *new_library;

	if ((new_library = slist_search(ffi_libraries,name)))
	{
		goto success;
	}

	if (!(new_library = ffi_library_open(name)))
	{
		// count not open the library
		badger_debug("badger_ffi_open: could not open the library %s\n",name);
		goto error;
	}

	if (slist_add(ffi_libraries,new_library->name,new_library) == -1)
	{
		// malloc failed
		goto error;
	}

success:
	badger_return_true(caller);
	return BADGER_SUCCESS;

error:
	badger_return_error(caller,"could not open the library");
	return BADGER_ERROR;
}

const badger_function_t badger_ffi_libraries_func = {"libraries",badger_ffi_libraries,badger_data_array,0};

int badger_ffi_libraries(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	badger_return_array(caller,slist_length(ffi_libraries));

	slist_node_t *next_node = ffi_libraries->head;

	while (next_node)
	{
		const ffi_library_t *lib = (const ffi_library_t *)(next_node->data);

		badger_return_string(caller,lib->name);
		next_node = next_node->next;
	}

	return BADGER_SUCCESS;
}

const badger_function_t badger_ffi_attach_function_func = {"attach_function",badger_ffi_attach_function,badger_data_boolean,4,{badger_data_string, badger_data_string, badger_data_array, badger_data_string}};

int badger_ffi_attach_function(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	size_t lib_name_length = badger_string_length(args);
	char lib_name[lib_name_length + 1];

	memcpy(lib_name,badger_string(args),lib_name_length);
	lib_name[lib_name_length] = '\0';

	ffi_library_t *lib;

	if (!(lib = slist_search(ffi_libraries,lib_name)))
	{
		// library not found
		badger_return_error(caller,"could not find the given library");
		return BADGER_ERROR;
	}

	size_t name_length = badger_string_length(args+1);
	char name[name_length + 1];

	memcpy(name,badger_string(args+1),name_length);
	name[name_length] = '\0';

	switch (ffi_library_detach(lib,name))
	{
		case 0:
			badger_debug("badger_ffi_attach_function: attaching new function %s\n",name);
			break;
		case 1:
			badger_debug("badger_ffi_attach_function: replacing function %s\n",name);
			break;
	}

	size_t func_argc = badger_array_length(args+2);
	const ffi_type *func_arg_types[func_argc];

	const badger_data_t *func_args = badger_array(args+2);
	unsigned int i;

	for (i=0;i<func_argc;i++)
	{
		size_t arg_name_length = badger_string_length(func_args+i);
		char arg_name[arg_name_length + 1];

		memcpy(arg_name,badger_string(func_args+i),arg_name_length);
		arg_name[arg_name_length] = '\0';

		if (!(func_arg_types[i] = ffi_types_parse(arg_name)))
		{
			// unknown type
			badger_return_error(caller,"unknown FFI argument type");
			return BADGER_ERROR;
		}
	}

	size_t ret_name_length = badger_string_length(args+3);
	char ret_name[ret_name_length + 1];

	memcpy(ret_name,badger_string(args+3),ret_name_length);
	ret_name[ret_name_length] = '\0';

	const ffi_type *func_ret_type;

	if (!(func_ret_type = ffi_types_parse(ret_name)))
	{
		// unknown type
		badger_return_error(caller,"unknown FFI return type");
		return BADGER_ERROR;
	}

	if (ffi_library_attach_function(lib,name,func_ret_type,func_argc,func_arg_types) == -1)
	{
		badger_return_error(caller,"could not find function");
		return BADGER_ERROR;
	}

	badger_return_true(caller);
	return BADGER_SUCCESS;
}

const badger_function_t badger_ffi_exposed_functions_func = {"exposed_functions",badger_ffi_exposed_functions,badger_data_array,1,{badger_data_string}};

int badger_ffi_exposed_functions(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	size_t name_length = badger_string_length(args);
	char name[name_length + 1];

	memcpy(name,badger_string(args),name_length);
	name[name_length] = '\0';

	const ffi_library_t *lib;

	if (!(lib = slist_search(ffi_libraries,name)))
	{
		// library not found
		badger_return_error(caller,"could not find the given library");
		return BADGER_ERROR;
	}

	badger_return_array(caller,slist_length(lib->functions));

	slist_node_t *next_node = lib->functions->head;

	while (next_node)
	{
		const ffi_function_t *func = (const ffi_function_t *)(next_node->data);

		badger_return_string(caller,func->name);
		next_node = next_node->next;
	}

	return BADGER_SUCCESS;
}

const badger_function_t badger_ffi_exposed_function_func = {"exposed_function",badger_ffi_exposed_function,badger_data_array,2,{badger_data_string, badger_data_string}};

int badger_ffi_exposed_function(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	size_t lib_name_length = badger_string_length(args);
	char lib_name[lib_name_length + 1];

	memcpy(lib_name,badger_string(args),lib_name_length);
	lib_name[lib_name_length] = '\0';

	const ffi_library_t *lib;

	if (!(lib = slist_search(ffi_libraries,lib_name)))
	{
		// library not found
		badger_return_error(caller,"could not find the given library");
		return BADGER_ERROR;
	}

	size_t name_length = badger_string_length(args+1);
	char name[name_length + 1];

	memcpy(name,badger_string(args+1),name_length);
	name[name_length] = '\0';

	const ffi_function_t *function;

	if (!(function = ffi_library_search(lib,name)))
	{
		// function not found
		badger_return_error(caller,"could not find the given function");
		return BADGER_ERROR;
	}

	badger_return_array(caller,2);

	size_t func_argc = function->argc;
	unsigned int i;

	badger_return_array(caller,function->argc);

	for (i=0;i<func_argc;i++)
	{
		badger_return_string(caller,ffi_types_name(function->arg_types[i]));
	}

	badger_return_string(caller,ffi_types_name(function->ret_type));
	return BADGER_SUCCESS;
}

const badger_function_t badger_ffi_invoke_func = {"invoke",badger_ffi_invoke,badger_data_any,3,{badger_data_string,badger_data_string,badger_data_array}};

int badger_ffi_invoke(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	size_t lib_name_length = badger_string_length(args);
	char lib_name[lib_name_length + 1];

	memcpy(lib_name,badger_string(args),lib_name_length);
	lib_name[lib_name_length] = '\0';

	const ffi_library_t *lib;

	if (!(lib = slist_search(ffi_libraries,lib_name)))
	{
		// library not found
		badger_return_error(caller,"could not find the given library");
		return BADGER_ERROR;
	}

	size_t name_length = badger_string_length(args+1);
	char name[name_length + 1];

	memcpy(name,badger_string(args+1),name_length);
	name[name_length] = '\0';

	const ffi_function_t *function;

	if (!(function = ffi_library_search(lib,name)))
	{
		// function not found
		badger_return_error(caller,"could not find the given function");
		return BADGER_ERROR;
	}

	size_t given_argc = badger_array_length(args+2);
	const badger_data_t *given_args = badger_array(args+2);

	switch (badger_ffi_function_valid(function,given_argc,given_args))
	{
		case BADGER_ERRNO_ARGC:
			badger_return_error(caller,"invalid number of arguments");
			return BADGER_ERROR;
		case BADGER_ERRNO_ARG_TYPE:
			badger_return_error(caller,"invalid argument type given");
			return BADGER_ERROR;
	}

	ffi_value_t func_args[given_argc];
	unsigned int i;

	for (i=0;i<given_argc;i++)
	{
		switch (badger_ffi_value_init(func_args+i,function->arg_types[i],given_args+i))
		{
			case BADGER_ERRNO_ARG_TYPE:
				badger_return_error(caller,"incompatible argument type");
			case BADGER_ERROR:
				goto cleanup_func_args;
		}
	}

	ffi_value_t func_ret;

	ffi_function_call(function,given_argc,func_args,&func_ret);

	switch (function->ret_type->type)
	{
		case FFI_TYPE_INT:
			badger_return_int(caller,func_ret.si);
			break;
		case FFI_TYPE_SINT8:
			badger_return_int(caller,func_ret.s8);
			break;
		case FFI_TYPE_SINT16:
			badger_return_int(caller,func_ret.s16);
			break;
		case FFI_TYPE_SINT32:
			badger_return_int(caller,func_ret.s32);
			break;
		case FFI_TYPE_SINT64:
			badger_return_int(caller,func_ret.s64);
			break;
		case FFI_TYPE_UINT8:
			badger_return_uint(caller,func_ret.u8);
			break;
		case FFI_TYPE_UINT16:
			badger_return_uint(caller,func_ret.u16);
			break;
		case FFI_TYPE_UINT32:
			badger_return_uint(caller,func_ret.u32);
			break;
		case FFI_TYPE_UINT64:
			badger_return_uint(caller,func_ret.u64);
			break;
		case FFI_TYPE_FLOAT:
			badger_return_float(caller,func_ret.f);
			break;
		case FFI_TYPE_DOUBLE:
		case FFI_TYPE_LONGDOUBLE:
			badger_return_float(caller,func_ret.fl);
			break;
		case FFI_TYPE_POINTER:
			badger_return_uint(caller,(uint64_t)func_ret.ptr);
			break;
	}

	ffi_value_clear(&func_ret);

	for (i=0;i<given_argc;i++)
	{
		badger_ffi_value_fini(func_args+i,function->arg_types[i],given_args+i);
	}
	return BADGER_SUCCESS;

cleanup_func_args:
	for (i=0;i<given_argc;i++)
	{
		badger_ffi_value_fini(func_args+i,function->arg_types[i],given_args+i);
	}

	return BADGER_ERROR;
}

const badger_function_t badger_ffi_malloc_func = {"malloc",badger_ffi_malloc,badger_data_uint,1,{badger_data_uint}};

int badger_ffi_malloc(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	size_t length = badger_uint(args);
	const void *ptr;

	if (!(ptr = calloc(length,1)))
	{
		badger_return_errno(caller);
		return BADGER_ERROR;
	}

	badger_return_uint(caller,(uint64_t)ptr);
	return BADGER_SUCCESS;
}

const badger_function_t badger_ffi_free_func = {"free",badger_ffi_free,badger_data_boolean,1,{badger_data_uint}};

int badger_ffi_free(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	void *ptr = (void *)badger_uint(args);

	free(ptr);

	badger_return_true(caller);
	return BADGER_SUCCESS;
}

const badger_function_t badger_ffi_read_func = {"read",badger_ffi_read,badger_data_any,4,{badger_data_uint,badger_data_string,badger_data_uint,badger_data_uint}};

int badger_ffi_read(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	const void *ptr = (const void *)(badger_uint(args));

	size_t type_name_length = badger_string_length(args+1);
	char type_name[type_name_length + 1];

	memcpy(type_name,badger_string(args+1),type_name_length);
	type_name[type_name_length] = '\0';

	const ffi_type *type;

	if (!(type = ffi_types_parse(type_name)))
	{
		badger_return_error(caller,"unknown badger FFI type");
		return BADGER_ERROR;
	}

	size_t offset = badger_uint(args+2);
	size_t length = badger_uint(args+3);
	unsigned int i;

	badger_return_array(caller,length);

	for (i=0;i<length;i++)
	{
		const ffi_value_t *value = (ptr + (type->size * (offset + i)));

		switch (type->type)
		{
			case FFI_TYPE_INT:
				badger_return_int(caller,value->si);
				break;
			case FFI_TYPE_SINT8:
				badger_return_int(caller,value->s8);
				break;
			case FFI_TYPE_SINT16:
				badger_return_int(caller,value->s16);
				break;
			case FFI_TYPE_SINT32:
				badger_return_int(caller,value->s32);
				break;
			case FFI_TYPE_SINT64:
				badger_return_int(caller,value->s64);
				break;
			case FFI_TYPE_UINT8:
				badger_return_uint(caller,value->u8);
				break;
			case FFI_TYPE_UINT16:
				badger_return_uint(caller,value->u16);
				break;
			case FFI_TYPE_UINT32:
				badger_return_uint(caller,value->u32);
				break;
			case FFI_TYPE_UINT64:
				badger_return_uint(caller,value->u64);
				break;
			case FFI_TYPE_FLOAT:
				badger_return_float(caller,value->f);
				break;
			case FFI_TYPE_DOUBLE:
			case FFI_TYPE_LONGDOUBLE:
				badger_return_float(caller,value->fl);
				break;
			case FFI_TYPE_POINTER:
				badger_return_uint(caller,(uint64_t)value->ptr);
				break;
		}
	}

	return BADGER_SUCCESS;
}

const badger_function_t badger_ffi_write_func = {"write",badger_ffi_write,badger_data_boolean,4,{badger_data_uint,badger_data_string,badger_data_uint,badger_data_array}};

int badger_ffi_write(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	void *ptr = (void *)(badger_uint(args));

	size_t type_name_length = badger_string_length(args+1);
	char type_name[type_name_length + 1];

	memcpy(type_name,badger_string(args+1),type_name_length);
	type_name[type_name_length] = '\0';

	const ffi_type *type;

	if (!(type = ffi_types_parse(type_name)))
	{
		badger_return_error(caller,"unknown badger FFI type");
		return BADGER_ERROR;
	}

	size_t offset = badger_uint(args+2);
	size_t length = badger_array_length(args+3);
	const badger_data_t *array = badger_array(args+3);
	unsigned int i;

	for (i=0;i<length;i++)
	{
		ffi_value_t *value = (ptr + (type->size * (offset + i)));

		badger_ffi_value_init(value,type,array+i);
	}

	badger_return_true(caller);
	return BADGER_SUCCESS;
}

const badger_function_t badger_ffi_close_func = {"close",badger_ffi_close,badger_data_boolean,1,{badger_data_string}};

int badger_ffi_close(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	size_t name_length = badger_string_length(args);
	char name[name_length + 1];

	memcpy(name,badger_string(args),name_length);
	name[name_length] = '\0';

	if (slist_remove(ffi_libraries,name) == -1)
	{
		// no libraries opened
		badger_return_error(caller,"could not remove library");
		return BADGER_ERROR;
	}

	badger_return_true(caller);
	return BADGER_SUCCESS;
}
