#ifndef _BADGER_CONFIG_H_
#define _BADGER_CONFIG_H_

#define BADGER_VERSION "${LIBRARY_VERSION}"

#cmakedefine DEBUG

// badger encoders
#cmakedefine ENCODERS_XOR
#cmakedefine ENCODERS_AES

// badger services
#cmakedefine SERVICES_SYS
#cmakedefine SERVICES_FS
#cmakedefine SERVICES_FFI

#endif
