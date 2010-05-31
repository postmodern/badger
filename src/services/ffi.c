#include <badger/services/ffi.h>
#include <badger/slist.h>

#include "private/ffi/types.h"
#include "private/ffi/library.h"

const badger_service_t badger_ffi_service = {
	"ffi",
	{
		&badger_ffi_open_func,
		&badger_ffi_libs_func,
		&badger_ffi_register_func,
		&badger_ffi_functions_func,
		&badger_ffi_close_func,
		NULL
	}
};

slist_t *ffi_libraries = NULL;

const badger_function_t badger_ffi_open_func = {"open",badger_ffi_open,badger_data_nil,1,{badger_data_string}};

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
		// library already loaded
		return BADGER_SUCCESS;
	}

	if (!(new_library = ffi_library_open(name)))
	{
		// count not open the library
		return BADGER_ERROR;
	}

	if (slist_add(ffi_libraries,new_library->name,new_library) == -1)
	{
		// malloc failed
		return BADGER_ERROR;
	}

	return BADGER_SUCCESS;
}

const badger_function_t badger_ffi_libs_func = {"libs",badger_ffi_libs,badger_data_array,0};

int badger_ffi_libs(int argc,const badger_data_t *args,badger_caller_t *caller)
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

const badger_function_t badger_ffi_register_func = {"register",badger_ffi_register,badger_data_array,4,{badger_data_string, badger_data_string, badger_data_array, badger_data_string}};

int badger_ffi_register(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	if (!ffi_libraries)
	{
		// no libraries opened
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

	for (i=0;i<argc;i++)
	{
		size_t arg_name_length = badger_string_length(func_args+i);
		char arg_name[arg_name_length + 1];

		memcpy(arg_name,badger_string(func_args+i),arg_name_length);
		arg_name[arg_name_length] = '\0';

		if (!(func_arg_types[i] = ffi_types_parse(arg_name)))
		{
			// unknown type
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
		return BADGER_ERROR;
	}

	if (ffi_library_register(lib,name,func_ret_type,func_argc,func_arg_types) == -1)
	{
		return BADGER_ERROR;
	}

	return BADGER_SUCCESS;
}

const badger_function_t badger_ffi_functions_func = {"functions",badger_ffi_functions,badger_data_array,1,{badger_data_string}};

int badger_ffi_functions(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	if (!ffi_libraries)
	{
		// no libraries opened
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

const badger_function_t badger_ffi_close_func = {"close",badger_ffi_close,badger_data_nil,1,{badger_data_string}};

int badger_ffi_close(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	if (!ffi_libraries)
	{
		return BADGER_ERROR;
	}

	size_t name_length = badger_string_length(args);
	char name[name_length + 1];

	memcpy(name,badger_string(args),name_length);
	name[name_length] = '\0';

	if (slist_remove(ffi_libraries,name) == -1)
	{
		return BADGER_ERROR;
	}

	return BADGER_SUCCESS;
}
