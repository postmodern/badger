#ifndef _BADGER_SERVICE_H_
#define _BADGER_SERVICE_H_

#include <badger/function.h>

struct badger_service
{
	const char *name;

	const badger_function_t *functions[];
};
typedef struct badger_service badger_service_t;

extern const badger_function_t * badger_service_search(const badger_service_t *service,const char *name);

#endif
