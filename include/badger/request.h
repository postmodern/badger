#ifndef _BADGER_REQUEST_H_
#define _BADGER_REQUEST_H_

#include <stdint.h>

#define BADGER_REQUEST_PING		0x01
#define BADGER_REQUEST_SERVICES		0x02
#define BADGER_REQUEST_FUNCTIONS	0x03
#define BADGER_REQUEST_CALL		0x04

typedef uint8_t badger_request_type;
typedef uint32_t badger_request_id;

#define BADGER_REQUEST_ID_MIN	1
#define BADGER_REQUEST_ID_MAX	4294967295

#endif
