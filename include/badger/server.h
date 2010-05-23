#ifndef _BADGER_SERVER_H_
#define _BADGER_SERVER_H_

#include <badger/slist.h>
#include <badger/service.h>

#include <poll.h>

typedef enum {
	badger_stdio,
	badger_tcp_listen,
	badger_tcp_connect,
	badger_udp_listen,
	badger_udp_connect
} badger_server_mode;

#define BADGER_MAX_SESSIONS	128

struct badger_server
{
	badger_server_mode mode;

	char *host;
	unsigned short port;

	int fd;
	struct pollfd *session_fds;

	slist_t *services;
};
typedef struct badger_server badger_server_t;

extern badger_server_t * badger_server_create(badger_server_mode mode);

extern int badger_server_listen(const char *host,unsigned short port);
extern int badger_server_close();

extern void badger_server_destroy();

#endif
