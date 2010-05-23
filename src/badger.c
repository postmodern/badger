#include <badger/config.h>
#include <badger/badger.h>
#include <badger/mempool.h>

#include <stdlib.h>
#include <time.h>

mempool_t *badger_global_mempool = NULL;

const char * badger_version()
{
	return BADGER_VERSION;
}

void badger_init()
{
	srand(time(NULL));

	badger_global_mempool = mempool_create();
}

void badger_fini()
{
	mempool_destroy(badger_global_mempool);

	badger_global_mempool = NULL;
}
