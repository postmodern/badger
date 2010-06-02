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

#ifndef _BADGER_DATA_H_
#define _BADGER_DATA_H_

#include <stdint.h>
#include <string.h>
#include <msgpack/object.h>
#include <msgpack/pack.h>

typedef enum
{
	badger_data_nil = MSGPACK_OBJECT_NIL,
	badger_data_boolean = MSGPACK_OBJECT_BOOLEAN,
	badger_data_uint = MSGPACK_OBJECT_POSITIVE_INTEGER,
	badger_data_int = MSGPACK_OBJECT_NEGATIVE_INTEGER,
	badger_data_double = MSGPACK_OBJECT_DOUBLE,
	badger_data_raw = MSGPACK_OBJECT_RAW,
	badger_data_string = MSGPACK_OBJECT_RAW,
	badger_data_array = MSGPACK_OBJECT_ARRAY,
	badger_data_map = MSGPACK_OBJECT_MAP,
	badger_data_any = -1 
} badger_data_type;

typedef msgpack_object badger_data_t;

static inline badger_data_type badger_type(const badger_data_t *data)
{
	return data->type;
}

extern const char * badger_data_type_name(badger_data_type type);

static inline unsigned int badger_boolean(const badger_data_t *data)
{
	return data->via.boolean;
}

static inline uint64_t badger_uint(const badger_data_t *data)
{
	return data->via.u64;
}

static inline int64_t badger_int(const badger_data_t *data)
{
	return data->via.i64;
}

static inline double badger_double(const badger_data_t *data)
{
	return data->via.dec;
}

static inline uint32_t badger_raw_length(const badger_data_t *data)
{
	return data->via.raw.size;
}

static inline const unsigned char * badger_raw(const badger_data_t *data)
{
	return (const unsigned char *)(data->via.raw.ptr);
}

static inline uint32_t badger_string_length(const badger_data_t *data)
{
	return data->via.raw.size;
}

static inline const char * badger_string(const badger_data_t *data)
{
	return (const char *)(data->via.raw.ptr);
}

static inline uint32_t badger_array_length(const badger_data_t *data)
{
	return data->via.array.size;
}

static inline const badger_data_t * badger_array(const badger_data_t *data)
{
	return data->via.array.ptr;
}

static inline const badger_data_t * badger_array_index(const badger_data_t *data,uint32_t index)
{
	if (index >= data->via.array.size)
	{
		return NULL;
	}

	return data->via.array.ptr + index;
}

static inline uint32_t badger_map_length(const badger_data_t *data)
{
	return data->via.map.size;
}

static inline const badger_data_t * badger_map_index(const badger_data_t *data,uint32_t index)
{
	if (index >= data->via.map.size)
	{
		return NULL;
	}

	return (const badger_data_t *)(data->via.map.ptr + index);
}

#endif
