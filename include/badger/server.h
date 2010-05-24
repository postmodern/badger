#ifndef _BADGER_SERVER_H_
#define _BADGER_SERVER_H_

#include <badger/encoders.h>
#include <badger/service.h>
#include <badger/slist.h>

#include <zmq.h>

struct badger_server
{
	char *uri;
	void *zmq_context;
	void *zmq_socket;

	badger_encoder_func encoder_func;
	void *encoder_data;

	slist_t *services;
};
typedef struct badger_server badger_server_t;

extern badger_server_t * badger_server_create();
extern int badger_server_register(badger_server_t *server,const badger_service_t *service);
extern int badger_server_open(badger_server_t *server,const char *uri);
extern void badger_server_listen(const badger_server_t *server);
extern int badger_server_close(badger_server_t *server);
extern void badger_server_destroy(badger_server_t *server);

#endif
