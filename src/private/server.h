#ifndef _PRIVATE_BADGER_SERVER_H_
#define _PRIVATE_BADGER_SERVER_H_

#include <badger/server.h>
#include <badger/request.h>

int badger_server_pull(badger_server_t *server);
int badger_server_decode(badger_server_t *server,const unsigned char *packet,size_t packet_size);
int badger_server_unpack(badger_server_t *server,const unsigned char *packed_payload,size_t packet_size);

int badger_server_pack(const badger_server_t *server,const char *payload,size_t payload_size);
int badger_server_encode(const badger_server_t *server,const unsigned char *packet,size_t packet_size);
int badger_server_push(const badger_server_t *server,const unsigned char *payload,size_t packet_size);

int badger_server_dispatch(badger_server_t *server,const msgpack_object *payload);

int badger_server_pong(badger_server_t *server,badger_request_id id,const msgpack_object *fields);
int badger_server_services(badger_server_t *server,badger_request_id id,const msgpack_object *fields);
int badger_server_functions(badger_server_t *server,badger_request_id id,const msgpack_object *fields);
int badger_server_prototype(badger_server_t *server,badger_request_id id,const msgpack_object *fields);
int badger_server_call(badger_server_t *server,badger_request_id id,const msgpack_object *fields);

#endif
