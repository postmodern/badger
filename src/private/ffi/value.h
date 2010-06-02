#ifndef _PRIVATE_FFI_VALUE_H_
#define _PRIVATE_FFI_VALUE_H_

#include "private/ffi/types.h"
#include <badger/data.h>

#include <stdint.h>

union ffi_value
{
	int8_t s8;
	uint8_t u8;
	int16_t s16;
	uint16_t u16;
	int32_t s32;
	uint32_t u32;
	int64_t s64;
	uint64_t u64;
	signed int si;
	unsigned int ui;
	signed long sl;
	unsigned long ul;
	signed long long sll;
	unsigned long long ull;
	void* ptr;
	float f;
	double fl;
};
typedef union ffi_value ffi_value_t;

void ffi_value_clear(ffi_value_t *value);

int badger_ffi_value_init(ffi_value_t *value,const ffi_type *type,const badger_data_t *data);
void badger_ffi_value_fini(ffi_value_t *value,const ffi_type *type,const badger_data_t *data);

#endif
