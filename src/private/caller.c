#include "private/caller.h"

void badger_caller_init(badger_caller_t *caller)
{
	msgpack_sbuffer_init(&(caller->return_buffer));
	msgpack_packer_init(&(caller->return_packer),&(caller->return_buffer),msgpack_sbuffer_write);

	msgpack_sbuffer_init(&(caller->yield_buffer));
	msgpack_packer_init(&(caller->yield_packer),&(caller->yield_buffer),msgpack_sbuffer_write);
}

void badger_caller_fini(badger_caller_t *caller)
{
	if (caller->yield_buffer.data)
	{
		// zero the yield buffer before freeing it
		memset(caller->yield_buffer.data,0,caller->yield_buffer.size);
	}
	msgpack_sbuffer_destroy(&(caller->yield_buffer));

	if (caller->return_buffer.data)
	{
		// zero the return buffer before freeing it
		memset(caller->return_buffer.data,0,caller->return_buffer.size);
	}
	msgpack_sbuffer_destroy(&(caller->return_buffer));
}
