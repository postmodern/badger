#include <badger/caller.h>

#include <msgpack/sbuffer.h>

void badger_yield(badger_caller_t *caller)
{
	if (caller->closure)
	{
		if (caller->yield_buffer.data)
		{
			// zero the yield buffer before re-clearing it
			memset(caller->yield_buffer.data,0,caller->yield_buffer.size);
		}

		msgpack_sbuffer_destroy(&(caller->yield_buffer));
		msgpack_sbuffer_init(&(caller->yield_buffer));
	}
}
