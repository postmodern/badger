#ifndef _BADGER_RPC_H_
#define _BADGER_RPC_H_

#include <badger/slist.h>
#include <bert/data.h>

typedef badger_slist_t badger_rpc_t;

badger_rpc_t * badger_rpc_create();
int badger_rpc_call(badger_rpc_t *rpc,const bert_data_t *call,bert_data_t **resp);
void badger_rpc_destroy(badger_rpc_t *rpc);

#endif
