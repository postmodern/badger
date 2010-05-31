#ifndef _PRIVATE_FFI_TYPES_H_
#define _PRIVATE_FFI_TYPES_H_

#include <ffi.h>

struct ffi_types_mapping
{
	const char *name;
	const ffi_type *type;
};

const ffi_type * ffi_types_parse(const char *name);
const char * ffi_types_name(const ffi_type *type);

#endif
