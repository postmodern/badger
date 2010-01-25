#include <badger/ffi/func.h>
#include <badger/errno.h>

#include <malloc.h>
#include <string.h>

badger_ffi_func_t * badger_ffi_func_create(const char *name,const ffi_type **args,size_t args_length,ffi_type *ret)
{
	badger_ffi_func_t *new_func;

	if (!(new_func = malloc(sizeof(badger_ffi_func_t))))
	{
		// malloc failed
		goto cleanup;
	}

	if (!(new_func->name = strdup(name)))
	{
		goto cleanup_func;
	}

	// make sure the address is not set
	new_func->address = NULL;

	new_func->args_length = args_length;

	if (!(new_func->args = malloc(sizeof(ffi_type *)*args_length)))
	{
		goto cleanup_name;
	}
	memcpy(new_func->args,args,sizeof(ffi_type *)*args_length);

	new_func->ret = ret;
	return new_func;

cleanup_name:
	free(new_func->name);
cleanup_func:
	free(new_func);
cleanup:
	return NULL;
}

int badger_ffi_func_call(const badger_ffi_func_t *func,void **args,ffi_arg *ret)
{
	return BADGER_ERRNO_FUNC;
}

void badger_ffi_func_destroy(badger_ffi_func_t *func)
{
	if (!func)
	{
		return;
	}

	free(func->name);
	free(func->args);
	free(func);
}
