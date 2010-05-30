#include <badger/service.h>
#include <badger/func.h>

#include <stdlib.h>
#include <string.h>

const badger_func_t * badger_service_search(const badger_service_t *service,const char *name)
{
	unsigned int i = 0;
	const badger_func_t *func;

	while ((func = service->funcs[i]))
	{
		if (!strcmp(name,func->name))
		{
			return func;
		}

		++i;
	}

	return NULL;
}
