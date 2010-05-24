#include <badger/service.h>
#include <badger/func.h>

#include <stdlib.h>
#include <string.h>

const badger_func_t * badger_service_search(const badger_service_t *service,const char *name)
{
	const badger_func_t *next_func = service->funcs;

	while (next_func->name)
	{
		if (!strcmp(name,next_func->name))
		{
			return next_func;
		}

		++next_func;
	}

	return NULL;
}
