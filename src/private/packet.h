#ifndef _PRIVATE_BADGER_PACKET_H_
#define _PRIVATE_BADGER_PACKET_H_

#include <sys/types.h>

int badger_packet_valid(const unsigned char *packet,size_t packet_size);

#endif
