#include <badger/services/ffi.h>
#include <badger/slist.h>

#include "private/ffi/types.h"
#include "private/ffi/value.h"
#include "private/ffi/library.h"
#include "private/util.h"

const badger_service_t badger_ffi_service = {
	"ffi",
	{
		&badger_ffi_open_func,
		&badger_ffi_libraries_func,
		&badger_ffi_attach_function_func,
		&badger_ffi_exposed_functions_func,
		&badger_ffi_exposed_function_func,
		&badger_ffi_invoke_func,
		&badger_ffi_close_func,
		NULL
	}
};

slist_t *ffi_libraries = NULL;

const badger_function_t badger_ffi_open_func = {"open",badger_ffi_open,badger_data_boolean,1,{badger_data_string}};

int badger_ffi_open(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	if (!ffi_libraries)
	{
		ffi_libraries = slist_create(slist_compare_strings,(slist_destroy_func)ffi_library_close);
	}

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
	if (!ffi_libraries)
	{
		badger_return_array(caller,0);
		return BADGER_SUCCESS;
	}

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
	if (!ffi_libraries)
	{
		// no libraries opened
		badger_return_error(caller,"no libraries have been loaded");
		return BADGER_ERROR;
	}

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
	if (!ffi_libraries)
	{
		// no libraries opened
		badger_return_error(caller,"no libraries have been loaded");
		return BADGER_ERROR;
	}

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
	if (!ffi_libraries)
	{
		// no libraries opened
		badger_return_error(caller,"no libraries have been loaded");
		return BADGER_ERROR;
	}

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
	if (!ffi_libraries)
	{
		// no libraries opened
		badger_return_error(caller,"no libraries have been loaded");
		return BADGER_ERROR;
	}

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

	const badger_data_t *given_args = badger_array(args+2);
	size_t func_argc = function->argc;

	if (badger_array_length(args+2) != func_argc)
	{
		// argc mismatch
		badger_return_error(caller,"invalid number of arguments");
		return BADGER_ERROR;
	}

	ffi_value_t func_args[func_argc];
	void *func_arg_ptrs[func_argc];

	unsigned int i;

	for (i=0;i<func_argc;i++)
	{
		// zero the function arg
		ffi_value_clear(func_args+i);
		func_arg_ptrs[i] = func_args+i;
	}

	for (i=0;i<func_argc;i++)
	{
		const badger_data_t *given_arg = given_args+i;

		size_t raw_length;
		unsigned char *raw_ptr;

		switch (function->arg_types[i]->type)
		{
			case FFI_TYPE_INT:
				switch (badger_type(given_arg))
				{
					case badger_data_int:
						func_args[i].ui = badger_int(given_arg);
						break;
					case badger_data_uint:
						func_args[i].ui = badger_uint(given_arg);
						break;
					default:
						// invalid given argument type
						badger_return_error(caller,"incompatible argument type for FFI int");
						goto cleanup_func_args;
				}
				break;
			case FFI_TYPE_UINT8:
				switch (badger_type(given_arg))
				{
					case badger_data_int:
						func_args[i].u8 = badger_int(given_arg);
						break;
					case badger_data_uint:
						func_args[i].u8 = badger_uint(given_arg);
						break;
					default:
						// invalid given argument type
						badger_return_error(caller,"incompatible argument type for FFI uint8");
						goto cleanup_func_args;
				}
				break;
			case FFI_TYPE_UINT16:
				switch (badger_type(given_arg))
				{
					case badger_data_int:
						func_args[i].u16 = badger_int(given_arg);
						break;
					case badger_data_uint:
						func_args[i].u16 = badger_uint(given_arg);
						break;
					default:
						// invalid given argument type
						badger_return_error(caller,"incompatible argument type for FFI uint16");
						goto cleanup_func_args;
				}
				break;
			case FFI_TYPE_UINT32:
				switch (badger_type(given_arg))
				{
					case badger_data_int:
						func_args[i].u32 = badger_int(given_arg);
						break;
					case badger_data_uint:
						func_args[i].u32 = badger_uint(given_arg);
						break;
					default:
						// invalid given argument type
						badger_return_error(caller,"incompatible argument type for FFI uint32");
						goto cleanup_func_args;
				}
				break;
			case FFI_TYPE_UINT64:
				switch (badger_type(given_arg))
				{
					case badger_data_int:
						func_args[i].u64 = badger_int(given_arg);
						break;
					case badger_data_uint:
						func_args[i].u64 = badger_uint(given_arg);
						break;
					default:
						// invalid given argument type
						badger_return_error(caller,"incompatible argument type for FFI uint64");
						goto cleanup_func_args;
				}
				break;
			case FFI_TYPE_SINT8:
				switch (badger_type(given_arg))
				{
					case badger_data_int:
						func_args[i].s8 = badger_int(given_arg);
						break;
					case badger_data_uint:
						func_args[i].s8 = badger_uint(given_arg);
						break;
					default:
						// invalid given argument type
						badger_return_error(caller,"incompatible argument type for FFI int8");
						goto cleanup_func_args;
				}
				break;
			case FFI_TYPE_SINT16:
				switch (badger_type(given_arg))
				{
					case badger_data_int:
						func_args[i].s16 = badger_int(given_arg);
						break;
					case badger_data_uint:
						func_args[i].s16 = badger_uint(given_arg);
						break;
					default:
						// invalid given argument type
						badger_return_error(caller,"incompatible argument type for FFI int16");
						goto cleanup_func_args;
				}
				break;
			case FFI_TYPE_SINT32:
				switch (badger_type(given_arg))
				{
					case badger_data_int:
						func_args[i].s32 = badger_int(given_arg);
						break;
					case badger_data_uint:
						func_args[i].s32 = badger_uint(given_arg);
						break;
					default:
						// invalid given argument type
						badger_return_error(caller,"incompatible argument type for FFI int32");
						goto cleanup_func_args;
				}
				break;
			case FFI_TYPE_SINT64:
				switch (badger_type(given_arg))
				{
					case badger_data_int:
						func_args[i].s64 = badger_int(given_arg);
						break;
					case badger_data_uint:
						func_args[i].s64 = badger_uint(given_arg);
						break;
					default:
						// invalid given argument type
						badger_return_error(caller,"incompatible argument type for FFI int64");
						goto cleanup_func_args;
				}
				break;
			case FFI_TYPE_FLOAT:
				switch (badger_type(given_arg))
				{
					case badger_data_float:
						func_args[i].f = badger_float(given_arg);
						break;
					default:
						// invalid given argument type
						badger_return_error(caller,"incompatible argument type for FFI float");
						goto cleanup_func_args;
				}
				break;
			case FFI_TYPE_DOUBLE:
			case FFI_TYPE_LONGDOUBLE:
				switch (badger_type(given_arg))
				{
					case badger_data_float:
						func_args[i].fl = badger_float(given_arg);
						break;
					default:
						// invalid given argument type
						badger_return_error(caller,"incompatible argument type for FFI double");
						goto cleanup_func_args;
				}
				break;
			case FFI_TYPE_POINTER:
				switch (badger_type(given_arg))
				{
					case badger_data_uint:
						func_args[i].ptr = (void *)badger_uint(given_arg);
						break;
					case badger_data_raw:
						raw_length = badger_raw_length(given_arg);

						if (!(raw_ptr = malloc(raw_length + 1)))
						{
							// malloc failed
							goto cleanup_func_args;
						}

						// copy the raw buffer and append a \0 byte to it (just in case)
						memcpy(raw_ptr,badger_raw(given_arg),raw_length);
						raw_ptr[raw_length] = '\0';

						func_args[i].ptr = raw_ptr;
						break;
					default:
						// invalid given argument type
						badger_return_error(caller,"incompatible argument type for FFI pointer");
						goto cleanup_func_args;
				}
				break;
			default:
				// unsupported FFI type
				badger_debug("badger_ffi_invoke: unknown FFI type (%u)\n",function->arg_types[i]->type);

				badger_return_error(caller,"unknown FFI type");
				goto cleanup_func_args;
		}
	}

	ffi_value_t func_ret;
	ffi_cif *cif = (ffi_cif *)&(function->cif);

	ffi_call(cif,FFI_FN(function->ptr),&func_ret,func_arg_ptrs);

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
	return BADGER_SUCCESS;

cleanup_func_args:
	for (i=0;i<func_argc;i++)
	{
		const badger_data_t *given_arg = given_args+i;

		switch (function->arg_types[i]->type)
		{
			case FFI_TYPE_POINTER:
				switch (badger_type(given_arg))
				{
					case badger_data_raw:
						// zero the copied raw buffer before freeing it
						memset(func_args[i].ptr,0,badger_raw_length(given_arg));
						free(func_args[i].ptr);
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}

		ffi_value_clear(func_args+i);
	}

	return BADGER_ERROR;
}

const badger_function_t badger_ffi_close_func = {"close",badger_ffi_close,badger_data_boolean,1,{badger_data_string}};

int badger_ffi_close(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	if (!ffi_libraries)
	{
		// no libraries opened
		badger_return_error(caller,"no libraries have been loaded");
		return BADGER_ERROR;
	}

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
