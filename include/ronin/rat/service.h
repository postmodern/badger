#ifndef _RONIN_RAT_SERVICE_H_
#define _RONIN_RAT_SERVICE_H_

#include <ronin/rat/list.h>
#include <ronin/rat/func.h>

struct ronin_rat_service
{
	char *name;

	ronin_rat_list_t *functions;
};
typedef struct ronin_rat_service ronin_rat_service_t;

ronin_rat_service_t * ronin_rat_service_create(const char *name);
const ronin_rat_func_t * ronin_rat_service_search(const ronin_rat_service_t *service,const char *name);
void ronin_rat_service_destroy(ronin_rat_service_t *service);

#endif
