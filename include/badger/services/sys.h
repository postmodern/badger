#ifndef _BADGER_SERVICES_SYS_H_
#define _BADGER_SERVICES_SYS_H_

#include <badger/service.h>

extern badger_service_t badger_sys_service;

extern int badger_sys_time(int argc,const badger_data_t *args,badger_caller_t *caller);

#endif
