#ifndef _BADGER_SERVICES_H_
#define _BADGER_SERVICES_H_

#include <badger/config.h>

#ifdef SERVICES_SYS
#include <badger/services/sys.h>
#endif

#ifdef SERVICES_FS
#include <badger/services/fs.h>
#endif

#ifdef SERVICES_FFI
#include <badger/services/ffi.h>
#endif

#endif
