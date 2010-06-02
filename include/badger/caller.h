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

#ifndef _BADGER_CALLER_H_
#define _BADGER_CALLER_H_

#include <badger/packet.h>
#include <badger/errno.h>

#include <msgpack/sbuffer.h>
#include <msgpack/pack.h>

struct badger_server;

struct badger_caller;
typedef struct badger_caller badger_caller_t;

extern int badger_return_nil(badger_caller_t *caller);
extern int badger_return_boolean(badger_caller_t *caller,unsigned int value);
extern int badger_return_true(badger_caller_t *caller);
extern int badger_return_false(badger_caller_t *caller);
extern int badger_return_uint(badger_caller_t *caller,unsigned int i);
extern int badger_return_uint8(badger_caller_t *caller,uint8_t i);
extern int badger_return_uint16(badger_caller_t *caller,uint16_t i);
extern int badger_return_uint32(badger_caller_t *caller,uint32_t i);
extern int badger_return_uint64(badger_caller_t *caller,uint64_t i);
extern int badger_return_int(badger_caller_t *caller,int i);
extern int badger_return_int8(badger_caller_t *caller,int8_t i);
extern int badger_return_int16(badger_caller_t *caller,int16_t i);
extern int badger_return_int32(badger_caller_t *caller,int32_t i);
extern int badger_return_int64(badger_caller_t *caller,int64_t i);
extern int badger_return_float(badger_caller_t *caller,double d);
extern int badger_return_raw(badger_caller_t *caller,const void *data,size_t length);
extern int badger_return_string(badger_caller_t *caller,const char *str);
extern int badger_return_array(badger_caller_t *caller,unsigned int length);
extern int badger_return_map(badger_caller_t *caller,unsigned int length);

extern int badger_return_error(badger_caller_t *caller,const char *message);
extern int badger_return_errno(badger_caller_t *caller);

extern int badger_yield_nil(badger_caller_t *caller);
extern int badger_yield_boolean(badger_caller_t *caller,unsigned int value);
extern int badger_yield_true(badger_caller_t *caller);
extern int badger_yield_false(badger_caller_t *caller);
extern int badger_yield_uint(badger_caller_t *caller,unsigned int i);
extern int badger_yield_uint8(badger_caller_t *caller,uint8_t i);
extern int badger_yield_uint16(badger_caller_t *caller,uint16_t i);
extern int badger_yield_uint32(badger_caller_t *caller,uint32_t i);
extern int badger_yield_uint64(badger_caller_t *caller,uint64_t i);
extern int badger_yield_int(badger_caller_t *caller,int i);
extern int badger_yield_int8(badger_caller_t *caller,int8_t i);
extern int badger_yield_int16(badger_caller_t *caller,int16_t i);
extern int badger_yield_int32(badger_caller_t *caller,int32_t i);
extern int badger_yield_int64(badger_caller_t *caller,int64_t i);
extern int badger_yield_float(badger_caller_t *caller,double d);
extern int badger_yield_raw(badger_caller_t *caller,const void *data,size_t length);
extern int badger_yield_string(badger_caller_t *caller,const char *str);
extern int badger_yield_array(badger_caller_t *caller,unsigned int length);
extern int badger_yield_map(badger_caller_t *caller,unsigned int length);

extern void badger_yield_open(badger_caller_t *caller,uint32_t args);
extern int badger_yield_close(badger_caller_t *caller);
extern int badger_yield(badger_caller_t *caller);

#endif
