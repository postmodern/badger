#ifndef _PRIVATE_FFI_FUNCTION_H_
#define _PRIVATE_FFI_FUNCTION_H_

#include <ffi.h>

struct ffi_function
{
	char *name;

	ffi_type *ret_type;

	int argc;
	ffi_type **arg_types;

	ffi_cif cif;
};
struct ffi_function ffi_function_t;

#endif
