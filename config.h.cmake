#ifndef _BADGER_CONFIG_H_
#define _BADGER_CONFIG_H_

#define BADGER_VERSION "${LIBRARY_VERSION}"

#cmakedefine DEBUG

// badger encoders
#cmakedefine XOR_ENCODER
#cmakedefine AES_ENCODER

// badger services
#cmakedefine SYS_SERVICE

#endif
