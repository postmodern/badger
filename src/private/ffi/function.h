#ifndef _PRIVATE_FFI_FUNCTION_H_
#define _PRIVATE_FFI_FUNCTION_H_

#include <ffi.h>

struct ffi_function
{
	char *name;
	void *ptr;

	const ffi_type *ret_type;

	size_t argc;
	const ffi_type **arg_types;

	ffi_cif cif;
};
typedef struct ffi_function ffi_function_t;

ffi_function_t * ffi_function_create(const char *name,void *ptr,const ffi_type *ret_type,size_t argc,const ffi_type **arg_types);
void ffi_function_destroy(ffi_function_t *function);

#endif
