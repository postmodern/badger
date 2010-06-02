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

#ifndef _BADGER_REQUEST_H_
#define _BADGER_REQUEST_H_

#include <stdint.h>

#define BADGER_REQUEST_PING		0x01
#define BADGER_REQUEST_SERVICES		0x02
#define BADGER_REQUEST_FUNCTIONS	0x03
#define BADGER_REQUEST_PROTOTYPE	0x04
#define BADGER_REQUEST_CALL		0x05

typedef uint8_t badger_request_type;
typedef uint32_t badger_request_id;

#define BADGER_REQUEST_ID_MIN	1
#define BADGER_REQUEST_ID_MAX	4294967295

#endif
