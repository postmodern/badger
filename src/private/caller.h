#ifndef _PRIVATE_BADGER_CALLER_H_
#define _PRIVATE_BADGER_CALLER_H_

#include <badger/caller.h>
#include "private/response.h"

struct badger_caller
{
	struct badger_server *server;
	unsigned returned:1;
	unsigned multi_return:1;
	unsigned yielded:1;

	badger_response_t ret;
	badger_response_t yield;
};

void badger_caller_init(badger_caller_t *caller,badger_request_id id,struct badger_server *server);
void badger_caller_returned(badger_caller_t *caller);
int badger_caller_return(badger_caller_t *caller);
void badger_caller_fini(badger_caller_t *caller);

#endif
