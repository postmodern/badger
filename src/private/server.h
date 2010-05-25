#ifndef _PRIVATE_BADGER_SERVER_H_
#define _PRIVATE_BADGER_SERVER_H_

#include <badger/server.h>

int badger_server_pull(const badger_server_t *server);
int badger_server_decode(const badger_server_t *server,zmq_msg_t *request);
int badger_server_unpack(const badger_server_t *server,const unsigned char *packed_payload,size_t packet_size);

int badger_server_pack(const badger_server_t *server,const char *payload,size_t payload_size);
int badger_server_encode(const badger_server_t *server,const unsigned char *packet,size_t packet_size);
int badger_server_push(const badger_server_t *server,const unsigned char *payload,size_t packet_size);

int badger_server_dispatch(const badger_server_t *server,const msgpack_object *payload);
int badger_server_call(const badger_server_t *server,const msgpack_object *fields);

#endif
