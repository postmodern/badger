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

#ifndef _BADGER_ENCODERS_H_
#define _BADGER_ENCODERS_H_

#include <badger/config.h>

#include <sys/types.h>

typedef void (*badger_encoder_func)(unsigned char *dest,const unsigned char *src,size_t length,void *data);
typedef void (*badger_decoder_func)(unsigned char *dest,const unsigned char *src,size_t length,void *data);

#ifdef ENCODERS_XOR

struct badger_xor
{
	size_t length;
	const unsigned char pad[];
};
typedef struct badger_xor badger_xor_t;

void badger_xor_encoder(unsigned char *dest,const unsigned char *src,size_t length,badger_xor_t *data);

#endif

#endif
