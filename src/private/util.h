#ifndef _BADGER_UTIL_H_
#define _BADGER_UTIL_H_

#include <badger/config.h>

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

#ifdef DEBUG
#define badger_debug(format,...)	fprintf(stderr,format,##__VA_ARGS__)
#else
#define badger_debug(format,...)
#endif

typedef uint32_t crc32_t;

crc32_t badger_crc32(const void *data,size_t size);

#endif
