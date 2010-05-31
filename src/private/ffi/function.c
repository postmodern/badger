#include "private/ffi/function.h"

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
	return new_function;

cleanup_arg_types:
	free(new_arg_types);
cleanup_name:
	free(new_name);
cleanup:
	return NULL;
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
