#include "private/caller.h"

void badger_caller_init(badger_caller_t *caller,badger_request_id id,struct badger_server *server)
{
	caller->server = server;
	caller->returned = 0;

	badger_response_init(&(caller->ret),id,BADGER_RESPONSE_RETURN);
	badger_response_init(&(caller->yield),id,BADGER_RESPONSE_YIELD);
}

void badger_caller_returned(badger_caller_t *caller)
{
	if (caller->returned)
	{
		badger_response_reset(&(caller->ret),BADGER_RESPONSE_RETURN);
		caller->returned = 0;
	}
	else
	{
		caller->returned = 1;
	}
}

void badger_caller_fini(badger_caller_t *caller)
{
	badger_response_clear(&(caller->ret));
	badger_response_clear(&(caller->yield));
}
