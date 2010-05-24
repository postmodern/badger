#ifndef _BADGER_UTIL_H_
#define _BADGER_UTIL_H_

#include <sys/types.h>
#include <stdint.h>

typedef uint32_t crc32_t;

crc32_t badger_crc32(const void *data,size_t size);

#endif
