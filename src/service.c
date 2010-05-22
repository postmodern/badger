#include <badger/service.h>
#include <badger/func.h>

#include <stdlib.h>
#include <string.h>

badger_service_t * badger_service_create(const char *name)
{
	badger_service_t *new_service;

	if (!(new_service = malloc(sizeof(badger_service_t))))
	{
		// malloc failed
		goto cleanup;
	}

	if (!(new_service->name = strdup(name)))
	{
		goto cleanup_service;
	}

	if (!(new_service->functions = slist_dict((slist_destroy_func)badger_func_destroy)))
	{
		goto cleanup_name;
	}

	return new_service;

cleanup_name:
	free(new_service->name);
cleanup_service:
	free(new_service);
cleanup:
	return NULL;
}

const badger_func_t * badger_service_search(const badger_service_t *service,const char *name)
{
	return (badger_func_t *)slist_search(service->functions,name);
}

void badger_service_destroy(badger_service_t *service)
{
	slist_destroy(service->functions);
	free(service->name);
	free(service);
}
