#ifndef _RONIN_FFI_FUNC_H_
#define _RONIN_FFI_FUNC_H_

#include <sys/types.h>
#include <ffi.h>

struct ronin_ffi_func
{
	char *name;
	void *address;

	size_t args_length;
	ffi_type **args;

	ffi_type *ret;
};
typedef struct ronin_ffi_func ronin_ffi_func_t;

extern ronin_ffi_func_t * ronin_ffi_func_create(const char *name,const ffi_type **args,size_t args_length,ffi_type *ret);
extern int ronin_ffi_func_call(const ronin_ffi_func_t *func,void **args,ffi_arg *ret);
extern void ronin_ffi_func_destroy(ronin_ffi_func_t *func);

#endif
