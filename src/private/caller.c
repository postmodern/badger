#include "private/caller.h"
#include "private/server.h"

void badger_caller_init(badger_caller_t *caller,badger_request_id id,struct badger_server *server)
{
	caller->server = server;
	caller->returned = 0;
	caller->multi_return = 0;
	caller->yielded = 0;

	badger_response_init(&(caller->ret),id,BADGER_RESPONSE_RETURN);
}

void badger_caller_returned(badger_caller_t *caller)
{
	if (caller->returned && !(caller->multi_return))
	{
		badger_response_reset(&(caller->ret),BADGER_RESPONSE_RETURN);
		caller->returned = 0;
	}
	else
	{
		caller->returned = 1;
	}
}

int badger_caller_return(badger_caller_t *caller)
{
	if (!(caller->returned))
	{
		badger_return_nil(caller);
	}

	// send back the return data
	return badger_server_pack(caller->server,caller->ret.buffer.data,caller->ret.buffer.size);
}

void badger_caller_fini(badger_caller_t *caller)
{
	badger_response_clear(&(caller->ret));

	if (caller->yielded)
	{
		badger_response_clear(&(caller->yield));
	}
}
