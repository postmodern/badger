#include <ronin/rat/service.h>
#include <ronin/rat/func.h>

#include <malloc.h>
#include <string.h>

ronin_rat_service_t * ronin_rat_service_create(const char *name)
{
	ronin_rat_service_t *new_service;

	if (!(new_service = malloc(sizeof(ronin_rat_service_t))))
	{
		// malloc failed
		goto cleanup;
	}

	if (!(new_service->name = strdup(name)))
	{
		goto cleanup_service;
	}

	if (!(new_service->functions = ronin_list_create((ronin_list_destroy_func)ronin_rat_func_destroy)))
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

const ronin_rat_func_t * ronin_rat_service_search(const ronin_rat_service_t *service,const char *name)
{
	return (ronin_rat_func_t *)ronin_list_search(service->functions,name);
}

void ronin_rat_service_destroy(ronin_rat_service_t *service)
{
	ronin_list_destroy(service->functions);
	free(service->name);
	free(service);
}
