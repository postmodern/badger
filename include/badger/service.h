#ifndef _BADGER_SERVICE_H_
#define _BADGER_SERVICE_H_

#include <badger/func.h>

struct badger_service
{
	const char *name;

	const badger_func_t *funcs[];
};
typedef struct badger_service badger_service_t;

extern const badger_func_t * badger_service_search(const badger_service_t *service,const char *name);

#endif
