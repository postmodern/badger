#ifndef _PRIVATE_BADGER_REQUEST_H_
#define _PRIVATE_BADGER_REQUEST_H_

#include <badger/request.h>

#include <stdint.h>
#include <msgpack/object.h>

typedef struct msgpack_object badger_request_t;

int badger_request_validate(const badger_request_t *request);

#endif
