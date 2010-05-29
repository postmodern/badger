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

/*
 * Build auxiliary table for parallel byte-at-a-time CRC-32.
 */
#define CRC32_POLY 0x04c11db7     /* AUTODIN II, Ethernet, & FDDI */

typedef uint32_t crc32_t;

void badger_crc32_init();
crc32_t badger_crc32(const void *data,size_t size);

#endif
