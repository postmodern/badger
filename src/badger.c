#include <badger/config.h>
#include <badger/badger.h>

#include <stdlib.h>
#include <time.h>

const char * badger_version()
{
	return BADGER_VERSION;
}

void badger_init()
{
	srand(time(NULL));
}

void badger_fini()
{
}
