#ifndef _BADGER_SERVICE_H_
#define _BADGER_SERVICE_H_

#include <badger/func.h>
#include <badger/slist.h>

struct badger_service
{
	char *name;

	slist_t *functions;
};
typedef struct badger_service badger_service_t;

badger_service_t * badger_service_create(const char *name);
const badger_func_t * badger_service_search(const badger_service_t *service,const char *name);
void badger_service_destroy(badger_service_t *service);

#endif
