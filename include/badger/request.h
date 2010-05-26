#ifndef _BADGER_REQUEST_H_
#define _BADGER_REQUEST_H_

#include <stdint.h>

#define BADGER_REQUEST_PING		0x01
#define BADGER_REQUEST_CALL		0x02

typedef uint8_t badger_request_type;
typedef uint32_t badger_request_id;

#endif
