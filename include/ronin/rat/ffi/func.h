#ifndef _RONIN_RAT_FUNC_H_
#define _RONIN_RAT_FUNC_H_

#include <sys/types.h>
#include <ffi.h>

struct ronin_rat_func
{
	char *name;
	void *address;

	size_t args_length;
	ffi_type **args;

	ffi_type *ret;
};
typedef struct ronin_rat_func ronin_rat_func_t;

ronin_rat_func_t * ronin_rat_func_create(const char *name,const ffi_type **args,size_t args_length,ffi_type *ret);
int ronin_rat_func_call(const ronin_rat_func_t *func,void **args,ffi_arg *ret);
void ronin_rat_func_destroy(ronin_rat_func_t *func);

#endif
