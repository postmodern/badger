#ifndef _PRIVATE_BADGER_RESPONSE_H_
#define _PRIVATE_BADGER_RESPONSE_H_

#include <badger/packet.h>
#include <badger/request.h>
#include <badger/response.h>

struct badger_server;

#include <msgpack/sbuffer.h>
#include <msgpack/pack.h>

struct badger_response
{
	badger_request_id id;
	badger_response_type type;

	struct msgpack_sbuffer buffer;
	struct msgpack_packer packer;
};
typedef struct badger_response badger_response_t;

void badger_response_init(badger_response_t *response,badger_request_id id,badger_response_type type);
void badger_response_clear(badger_response_t *response);

#endif
