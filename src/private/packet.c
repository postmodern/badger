/*
 * Badger - A fear-less, robust and portable systems management solution.
 *
 * Copyright (c) 2010 Hal Brodigan (postmodern.mod3 at gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#include <badger/packet.h>
#include "private/packet.h"
#include "private/util.h"

int badger_packet_valid(const unsigned char *packet,size_t packet_size)
{
	if (packet_size < BADGER_PACKET_MINSIZE)
	{
		// short packet
		badger_debug("badger_packet_valid: the received badger packet was less than BADGER_PACKET_MINSIZE\n");
		return 0;
	}

	const void *packed_payload = badger_packet_payload(packet);
	size_t payload_size = (packet_size - BADGER_PACKET_HDRSIZE);

	uint8_t packet_version = badger_packet_version(packet);

	if (packet_version != BADGER_PROTOCOL_VERSION)
	{
		// unsupported protocol
		badger_debug("badger_packet_valid: the received badger packet had an unknown Badger Protocol version (%u)\n",packet_version);
		return 0;
	}

	crc32_t claimed_checksum = badger_packet_checksum(packet);
	crc32_t actual_checksum = badger_crc32(packed_payload,payload_size);

	if (claimed_checksum != actual_checksum)
	{
		// corrupted packets
		badger_debug("badger_packet_valid: the received badger packet had an invalid checksum (%u), expected (%u)\n",claimed_checksum,actual_checksum);
		return 0;
	}

	return 1;
}
