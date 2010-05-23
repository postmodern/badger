#ifndef _BADGER_H_
#define _BADGER_H_

#include <badger/mempool.h>

#include <assert.h>

mempool_t *badger_global_mempool;

extern const char * badger_version();

extern void badger_init();
extern void badger_fini();

static void * badger_malloc(size_t size)
{
	assert(badger_global_mempool != NULL);

	return mempool_alloc(badger_global_mempool,size);
}

static void badger_free(void *ptr)
{
	assert(badger_global_mempool != NULL);

	mempool_free(badger_global_mempool,ptr);
}

#endif
