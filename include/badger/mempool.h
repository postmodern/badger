#ifndef _MEMPOOL_H_
#define _MEMPOOL_H_

#include "slist.h"

struct mempool_chunk
{
	size_t size;
	void *ptr;
};

struct mempool_chunk * mempool_chunk_create(size_t size);
void mempool_chunk_destroy(struct mempool_chunk *chunk);

typedef struct slist mempool_t;

static inline mempool_t * mempool_create()
{
	return slist_create(slist_compare_ints,(slist_destroy_func)mempool_chunk_destroy);
}

int mempool_alloc(mempool_t *mempool,size_t size);

static inline void mempool_destroy(mempool_t *mempool)
{
	slist_destroy(mempool);
}

#endif
