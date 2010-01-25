#ifndef _BADGER_FFI_FUNC_H_
#define _BADGER_FFI_FUNC_H_

#include <sys/types.h>
#include <ffi.h>

struct badger_ffi_func
{
	char *name;
	void *address;

	size_t args_length;
	ffi_type **args;

	ffi_type *ret;
};
typedef struct badger_ffi_func badger_ffi_func_t;

extern badger_ffi_func_t * badger_ffi_func_create(const char *name,const ffi_type **args,size_t args_length,ffi_type *ret);
extern int badger_ffi_func_call(const badger_ffi_func_t *func,void **args,ffi_arg *ret);
extern void badger_ffi_func_destroy(badger_ffi_func_t *func);

#endif
