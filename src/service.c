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

	if (!(new_service->functions = slist_create(slist_compare_strings,(slist_destroy_func)badger_func_destroy)))
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

int badger_service_register(badger_service_t *service,const char *name,badger_func_ptr ptr,int argc,msgpack_object_type *arg_types)
{
	badger_func_t *new_func;

	if (!(new_func = badger_func_create(name,ptr,argc,arg_types)))
	{
		// malloc failed
		goto cleanup;
	}

	if (slist_add(service->functions,new_func->name,new_func) == -1)
	{
		goto cleanup_func;
	}

	return 0;

cleanup_func:
	badger_func_destroy(new_func);
cleanup:
	return -1;
}

const badger_func_t * badger_service_search(const badger_service_t *service,const char *name)
{
	return (badger_func_t *)slist_search(service->functions,name);
}

void badger_service_destroy(badger_service_t *service)
{
	slist_destroy(service->functions);
	free(service->name);

	// zero the service fields before freeing it
	service->functions = NULL;
	service->name = NULL;

	free(service);
}
