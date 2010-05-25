#include <badger/caller.h>
#include "private/server.h"

#include <msgpack/sbuffer.h>

int badger_yield(badger_caller_t *caller)
{
	if (!caller->closure)
	{
		return 0;
	}

	int ret;

	// pack, encode and push the yield data
	ret = badger_server_pack(caller->server,caller->yield_buffer.data,caller->yield_buffer.size);

	if (caller->yield_buffer.data)
	{
		// zero the yield buffer before re-clearing it
		memset(caller->yield_buffer.data,0,caller->yield_buffer.size);
	}

	// re-create the yield buffer
	msgpack_sbuffer_destroy(&(caller->yield_buffer));
	msgpack_sbuffer_init(&(caller->yield_buffer));

	return ret;
}
