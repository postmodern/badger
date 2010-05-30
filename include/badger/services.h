#ifndef _BADGER_SERVICES_H_
#define _BADGER_SERVICES_H_

#include <badger/config.h>

#ifdef SYS_SERVICE
#include <badger/services/sys.h>
#endif

#ifdef FS_SERVICE
#include <badger/services/fs.h>
#endif

#endif
