#ifndef _BADGER_CALLER_H_
#define _BADGER_CALLER_H_

#include <badger/packet.h>

#include <msgpack/sbuffer.h>
#include <msgpack/pack.h>

struct badger_server;

struct badger_caller
{
	const struct badger_server *server;

	badger_packet_id id;
	unsigned closure:1;

	msgpack_sbuffer ret_buffer;
	msgpack_packer ret_packer;

	msgpack_sbuffer yield_buffer;
	msgpack_packer yield_packer;
};
typedef struct badger_caller badger_caller_t;

#endif
