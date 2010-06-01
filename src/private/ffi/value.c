#include "private/ffi/value.h"

#include <string.h>

void ffi_value_clear(ffi_value_t *value)
{
	memset(value,0,sizeof(ffi_value_t));
}
