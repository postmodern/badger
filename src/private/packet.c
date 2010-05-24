#include <badger/packet.h>
#include "private/util.h"

#include <malloc.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int badger_packet_valid(const unsigned char *packet,size_t packet_size)
{
	const void *packed_payload = (packet + 1 + sizeof(crc32_t));
	size_t payload_size = (packet_size - 1 - sizeof(crc32_t));

	uint8_t packet_version = *packet;

	if (packet_version != BADGER_PROTOCOL_VERSION)
	{
		// unsupported protocol
		return 0;
	}

	crc32_t claimed_checksum = ntohl(*(crc32_t *)(packet + 1));
	crc32_t actual_checksum = badger_crc32(packed_payload,payload_size);

	if (claimed_checksum != actual_checksum)
	{
		// ignore corrupted packets
		return 0;
	}

	return 1;
}
