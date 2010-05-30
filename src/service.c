#include <badger/service.h>

#include <stdlib.h>
#include <string.h>

const badger_function_t * badger_service_search(const badger_service_t *service,const char *name)
{
	unsigned int i = 0;
	const badger_function_t *function;

	while ((function = service->functions[i]))
	{
		if (!strcmp(name,function->name))
		{
			return function;
		}

		++i;
	}

	return NULL;
}
