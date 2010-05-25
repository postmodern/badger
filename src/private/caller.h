#ifndef _PRIVATE_BADGER_CALLER_H_
#define _PRIVATE_BADGER_CALLER_H_

#include <badger/caller.h>

void badger_caller_init(badger_caller_t *caller,badger_packet_id id,unsigned int closure,const struct badger_server *server);
void badger_caller_fini(badger_caller_t *caller);

#endif
