#include <badger/mempool.h>

#include <malloc.h>
#include <string.h>

struct mempool_chunk * mempool_chunk_create(size_t size)
{
	void *new_ptr;

	// allocate and zero the memory
	if (!(new_ptr = calloc(size,1)))
	{
		return NULL;
	}

	struct mempool_chunk *new_chunk;

	if (!(new_chunk = malloc(sizeof(struct mempool_chunk))))
	{
		free(new_ptr);
		return NULL;
	}

	new_chunk->size = size;
	new_chunk->ptr = new_ptr;
	return new_chunk;
}

void mempool_chunk_destroy(struct mempool_chunk *chunk)
{
	// zero the memory before freeing it
	memset(chunk->ptr,0,chunk->size);
	free(chunk->ptr);

	// zero the chunk fields before freeing it
	chunk->ptr = NULL;
	chunk->size = 0;

	free(chunk);
}

void * mempool_alloc(mempool_t *mempool,size_t size)
{
	struct mempool_chunk *new_chunk;

	if (!(new_chunk = mempool_chunk_create(size)))
	{
		return NULL;
	}

	if (slist_add(mempool,new_chunk->ptr,new_chunk) == -1)
	{
		mempool_chunk_destroy(new_chunk);
		return NULL;
	}

	return new_chunk->ptr;
}
