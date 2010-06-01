#ifndef _PRIVATE_FFI_VALUE_H_
#define _PRIVATE_FFI_VALUE_H_

#include "private/ffi/function.h"

union ffi_value
{
	signed char s8;
	unsigned char u8;
	signed short s16;
	unsigned short u16;
	signed int s32;
	unsigned int u32;
	signed long long i64;
	unsigned long long u64;
	signed long sl;
	unsigned long ul;
	void* ptr;
	float f32;
	double f64;
};
typedef union ffi_value ffi_value_t;

#endif
