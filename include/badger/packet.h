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
