#ifndef _PRIVATE_BADGER_SERVER_H_
#define _PRIVATE_BADGER_SERVER_H_

#include <badger/server.h>

int badger_server_recv(const badger_server_t *server);
int badger_server_send(const badger_server_t *server,const msgpack_object *payload);
int badger_server_process(const badger_server_t *server,const msgpack_object *payload);

#endif
