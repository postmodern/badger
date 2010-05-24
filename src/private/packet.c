#include <badger/packet.h>
#include "private/packet.h"
#include "private/util.h"

int badger_packet_valid(const unsigned char *packet,size_t packet_size)
{
	if (packet_size < BADGER_PACKET_MINSIZE)
	{
		// short packet
		return 0;
	}

	const void *packed_payload = badger_packet_payload(packet);
	size_t payload_size = (packet_size - BADGER_PACKET_HDRSIZE);

	uint8_t packet_version = badger_packet_version(packet);

	if (packet_version != BADGER_PROTOCOL_VERSION)
	{
		// unsupported protocol
		return 0;
	}

	crc32_t claimed_checksum = badger_packet_checksum(packet);
	crc32_t actual_checksum = badger_crc32(packed_payload,payload_size);

	if (claimed_checksum != actual_checksum)
	{
		// ignore corrupted packets
		return 0;
	}

	return 1;
}
