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

#ifndef _BADGER_PACKET_H_
#define _BADGER_PACKET_H_

/*
 * A Badger packet consists of the Badger Protocol version, a checksum and
 * a payload.
 *
 * The Badger Protocol version simply denotes what version of the
 * Badger Protocol the client or the server are speaking. The version number
 * is written as a single byte.
 *
 * The checksum is a simple CRC32 checksum of the payload. The CRC32
 * checksum is written as a network packed 4-byte integer.
 *
 * The payload is represented as a packed msgpack Array object.
 *
 * The first two elements of the Array must always be the following:
 *   1. The ID of the packet.
 *   2. The type of the payload.
 *
 */

#include <stdint.h>

#define BADGER_PROTOCOL_VERSION		0x01

#define BADGER_PACKET_HDRSIZE		(1 + 4)
#define BADGER_PACKET_MINSIZE		(BADGER_PACKET_HDRSIZE + 3)

#endif
