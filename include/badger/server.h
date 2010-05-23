#ifndef _BADGER_SERVER_H_
#define _BADGER_SERVER_H_

#include <badger/slist.h>
#include <badger/service.h>

#include <zmq.h>

struct badger_server
{
	char *uri;
	void *zmq_context;
	void *zmq_socket;

	slist_t *services;
};
typedef struct badger_server badger_server_t;

extern badger_server_t * badger_server_create();
extern int badger_server_open(badger_server_t *server,const char *uri);
extern int badger_server_listen(const badger_server_t *server);
extern int badger_server_close(badger_server_t *server);
extern void badger_server_destroy(badger_server_t *server);

#endif
