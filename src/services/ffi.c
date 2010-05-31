#include <badger/services/ffi.h>
#include <badger/slist.h>

#include "private/ffi/library.h"

const badger_service_t badger_ffi_service = {
	"ffi",
	{
		&badger_ffi_open_func,
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

const badger_function_t badger_ffi_close_func = {"open",badger_ffi_close,badger_data_nil,1,{badger_data_string}};

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
