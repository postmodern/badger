#ifndef _BADGER_RESPONSE_H_
#define _BADGER_RESPONSE_H_

#include <stdint.h>

#define BADGER_RESPONSE_PONG		0x41
#define BADGER_RESPONSE_YIELD		0x42
#define BADGER_RESPONSE_RETURN		0x43

#define BADGER_RESPONSE_ETYPE		0xff
#define BADGER_RESPONSE_ESHORT		0xfe
#define BADGER_RESPONSE_ENOSERVICE 	0xfd
#define BADGER_RESPONSE_ENOFUNC	 	0xfc
#define BADGER_RESPONSE_EARGC		0xfb

typedef uint8_t badger_response_type;

#endif
