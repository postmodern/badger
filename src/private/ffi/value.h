#ifndef _PRIVATE_FFI_VALUE_H_
#define _PRIVATE_FFI_VALUE_H_

#include "private/ffi/function.h"

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
	float f32;
	double f64;
};
typedef union ffi_value ffi_value_t;

#endif
