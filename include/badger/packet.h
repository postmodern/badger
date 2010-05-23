#ifndef _BADGER_PACKET_H_
#define _BADGER_PACKET_H_

/*
 * A Badger packet consists of a checksum and a payload.
 *
 * The checksum is a simple CRC32 checksum of the payload.
 *
 * The payload is represented as a packed msgpack Array object.
 *
 * The first two elements of the Array must always be the following:
 *   1. The type of the payload.
 *   2. The timestamp of the packet.
 *
 * The type of the payload may be one of the following:
 *   * BADGER_PACKET_PING - Represents a ping sent to the server.
 *   * BADGER_PACKET_PONG - Represents a pong in response to a previous ping.
 *   * BADGER_PACKET_CALL - Represents a function call being sent to the
 *     server.
 *   * BADGER_PACKET_RETURN - Represents a return value from a previous
 *     function call, being returned from the server.
 *   * BADGER_PACKET_ESHORT - The last received packet was shorter than
 *     BADGER_PACKET_MINSIZE or was missing expected payload elements.
 *   * BADGER_PACKET_ECHECKSUM - The last received packet had an incorrect
 *     checksum.
 *   * BADGER_PACKET_ETYPE - The last received packet had an unknown type.
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

#include <badger/util.h>

#include <stdint.h>

typedef uint8_t badger_packet_type;

#define BADGER_PACKET_PING		0x01
#define BADGER_PACKET_PONG		0x02
#define BADGER_PACKET_CALL		0x03
#define BADGER_PACKET_RETURN		0x04
#define BADGER_PACKET_ESHORT		0xff
#define BADGER_PACKET_ECHECKSUM		0xfe
#define BADGER_PACKET_ETYPE		0xfd
#define BADGER_PACKET_ENOSERVICE 	0xfc
#define BADGER_PACKET_ENOFUNC	 	0xfb
#define BADGER_PACKET_EARGC		0xfa

#define BADGER_PACKET_MINSIZE		(sizeof(crc32_t) + 7)

#endif