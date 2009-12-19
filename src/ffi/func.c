#include <ronin/rat/ffi/func.h>
#include <ronin/rat/errno.h>

#include <malloc.h>
#include <string.h>

ronin_rat_func_t * ronin_rat_func_create(const char *name,const ffi_type **args,size_t args_length,ffi_type *ret)
{
	ronin_rat_func_t *new_func;

	if (!(new_func = malloc(sizeof(ronin_rat_func_t))))
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

int ronin_rat_func_call(const ronin_rat_func_t *func,void **args,ffi_arg *ret)
{
	return RONIN_RAT_ERRNO_FUNC;
}

void ronin_rat_func_destroy(ronin_rat_func_t *func)
{
	if (!func)
	{
		return;
	}

	free(func->name);
	free(func->args);
	free(func);
}
