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
