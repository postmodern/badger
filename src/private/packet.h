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
