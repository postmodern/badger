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
 * The type of the payload may be one of the following:
 *   * BADGER_PACKET_PING - Represents a ping sent to the server.
 *   * BADGER_PACKET_PONG - Represents a pong in response to a previous ping.
 *   * BADGER_PACKET_CALL - Represents a function call being sent to the
 *     server.
 *   * BADGER_PACKET_RETURN - Represents a return value from a previous
 *     function call, being returned from the server.
 *   * BADGER_PACKET_ETYPE - The last received packet had an unknown type.
 *   * BADGER_PACKET_ESHORT - The last received packet did not have the
 *     expected number of fields.
 *   * BADGER_PACKET_ENOSERVICE - The last received call packet referenced an
 *     unknown service.
 *   * BADGER_PACKET_ENOFUNC - The last received call packet referenced an
 *     unknown function.
 *   * BADGER_PACKET_EARGC - The last received call packet had an invalid
 *     number of arguments.
 *
 * If the type of the payload is BADGER_PACKET_CALL, than the next three
 * elements of the Array must be the following:
 *   3. The service that the function is registered within.
 *   4. The name of the function to call.
 *   5. The Array of arguments to pass to the function.
 *
 * If the type key is set to BADGER_PACKET_RETURN, then the third
 * element of the Array must be the following:
 *   3. The return-value from a previous function call.
 */

#include <stdint.h>

#define BADGER_PROTOCOL_VERSION		0x01

#define BADGER_PACKET_HDRSIZE		(1 + 4)
#define BADGER_PACKET_MINSIZE		(BADGER_PACKET_HDRSIZE + 7)

#endif
