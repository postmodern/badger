#ifndef _PRIVATE_BADGER_PACKET_H_
#define _PRIVATE_BADGER_PACKET_H_

#include <badger/packet.h>
#include "private/util.h"

#include <stdint.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

static inline uint8_t badger_packet_version(const unsigned char *packet)
{
	return *packet;
}

static inline crc32_t badger_packet_checksum(const unsigned char *packet)
{
	return ntohl(*(crc32_t *)(packet + 1));
}

static inline const unsigned char * badger_packet_payload(const unsigned char *packet)
{
	return packet + BADGER_PACKET_HDRSIZE;
}

int badger_packet_valid(const unsigned char *packet,size_t packet_size);

#endif
