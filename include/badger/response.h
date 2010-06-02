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

#ifndef _BADGER_RESPONSE_H_
#define _BADGER_RESPONSE_H_

#include <stdint.h>

#define BADGER_RESPONSE_PONG		0x41
#define BADGER_RESPONSE_SERVICES	0x42
#define BADGER_RESPONSE_FUNCTIONS	0x43
#define BADGER_RESPONSE_PROTOTYPE	0x44
#define BADGER_RESPONSE_YIELD		0x45
#define BADGER_RESPONSE_RETURN		0x46
#define BADGER_RESPONSE_ERROR		0x47

#define BADGER_RESPONSE_ETYPE		0xff
#define BADGER_RESPONSE_ESHORT		0xfe
#define BADGER_RESPONSE_ENOSERVICE 	0xfd
#define BADGER_RESPONSE_ENOFUNC	 	0xfc
#define BADGER_RESPONSE_EARGC		0xfb

typedef uint8_t badger_response_type;

#endif
