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
	badger_data_map = MSGPACK_OBJECT_MAP
} badger_data_type;

typedef msgpack_object badger_data_t;

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

typedef msgpack_packer badger_ret_t;

static inline int badger_return_nil(badger_ret_t *ret)
{
	return msgpack_pack_nil(ret);
}

static inline int badger_return_boolean(badger_ret_t *ret,unsigned int value)
{
	if (value)
	{
		return msgpack_pack_true(ret);
	}
	else
	{
		return msgpack_pack_false(ret);
	}
}

static inline int badger_return_true(badger_ret_t *ret)
{
	return msgpack_pack_true(ret);
}

static inline int badger_return_false(badger_ret_t *ret)
{
	return msgpack_pack_false(ret);
}

static inline int badger_return_uint(badger_ret_t *ret,unsigned int i)
{
	return msgpack_pack_unsigned_int(ret,i);
}

static inline int badger_return_uint8(badger_ret_t *ret,uint8_t i)
{
	return msgpack_pack_uint8(ret,i);
}

static inline int badger_return_uint16(badger_ret_t *ret,uint16_t i)
{
	return msgpack_pack_uint16(ret,i);
}

static inline int badger_return_uint32(badger_ret_t *ret,uint32_t i)
{
	return msgpack_pack_uint32(ret,i);
}

static inline int badger_return_uint64(badger_ret_t *ret,uint64_t i)
{
	return msgpack_pack_uint64(ret,i);
}

static inline int badger_return_int(badger_ret_t *ret,int i)
{
	return msgpack_pack_int(ret,i);
}

static inline int badger_return_int8(badger_ret_t *ret,int8_t i)
{
	return msgpack_pack_int8(ret,i);
}

static inline int badger_return_int16(badger_ret_t *ret,int16_t i)
{
	return msgpack_pack_int16(ret,i);
}

static inline int badger_return_int32(badger_ret_t *ret,int32_t i)
{
	return msgpack_pack_int32(ret,i);
}

static inline int badger_return_int64(badger_ret_t *ret,int64_t i)
{
	return msgpack_pack_int64(ret,i);
}

static inline int badger_return_double(badger_ret_t *ret,double d)
{
	return msgpack_pack_double(ret,d);
}

static inline int badger_return_raw(badger_ret_t *ret,const void *data,size_t length)
{
	msgpack_pack_raw(ret,length);

	return msgpack_pack_raw_body(ret,data,length);
}

static inline int badger_return_string(badger_ret_t *ret,const char *str)
{
	return badger_return_raw(ret,str,strlen(str));
}

static inline int badger_return_array(badger_ret_t *ret,unsigned int length)
{
	return msgpack_pack_array(ret,length);
}

static inline int badger_return_map(badger_ret_t *ret,unsigned int length)
{
	return msgpack_pack_map(ret,length);
}

#endif
