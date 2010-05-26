#ifndef _BADGER_SERVER_H_
#define _BADGER_SERVER_H_

#include <badger/encoders.h>
#include <badger/service.h>
#include <badger/slist.h>

#include <zmq.h>

typedef enum
{
	badger_mode_connect,
	badger_mode_listen,
	badger_mode_none
} badger_mode;

struct badger_server
{
	badger_mode mode;
	char *uri;

	unsigned int running;
	void *zmq_context;
	void *zmq_socket;

	badger_encoder_func encoder_func;
	void *encoder_data;

	badger_decoder_func decoder_func;
	void *decoder_data;

	slist_t *services;
};
typedef struct badger_server badger_server_t;

extern badger_server_t * badger_server_create();
extern int badger_server_register(badger_server_t *server,const badger_service_t *service);
extern void badger_server_encoder(badger_server_t *server,badger_encoder_func encoder,void *data);
extern void badger_server_decoder(badger_server_t *server,badger_decoder_func decoder,void *data);
extern int badger_server_open(badger_server_t *server,badger_mode mode,const char *uri);
extern void badger_server_loop(badger_server_t *server);
extern void badger_server_break(badger_server_t *server);
extern int badger_server_close(badger_server_t *server);
extern void badger_server_destroy(badger_server_t *server);

#endif
