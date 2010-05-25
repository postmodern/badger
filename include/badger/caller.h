#ifndef _BADGER_CALLER_H_
#define _BADGER_CALLER_H_

#include <badger/packet.h>

#include <msgpack/sbuffer.h>
#include <msgpack/pack.h>

struct badger_server;

struct badger_caller
{
	const struct badger_server *server;

	badger_packet_id id;
	unsigned closure:1;

	msgpack_sbuffer return_buffer;
	msgpack_packer return_packer;

	msgpack_sbuffer yield_buffer;
	msgpack_packer yield_packer;
};
typedef struct badger_caller badger_caller_t;

static inline int badger_return_nil(badger_caller_t *caller)
{
	return msgpack_pack_nil(&(caller->return_packer));
}

static inline int badger_return_boolean(badger_caller_t *caller,unsigned int value)
{
	if (value)
	{
		return msgpack_pack_true(&(caller->return_packer));
	}
	else
	{
		return msgpack_pack_false(&(caller->return_packer));
	}
}

static inline int badger_return_true(badger_caller_t *caller)
{
	return msgpack_pack_true(&(caller->return_packer));
}

static inline int badger_return_false(badger_caller_t *caller)
{
	return msgpack_pack_false(&(caller->return_packer));
}

static inline int badger_return_uint(badger_caller_t *caller,unsigned int i)
{
	return msgpack_pack_unsigned_int(&(caller->return_packer),i);
}

static inline int badger_return_uint8(badger_caller_t *caller,uint8_t i)
{
	return msgpack_pack_uint8(&(caller->return_packer),i);
}

static inline int badger_return_uint16(badger_caller_t *caller,uint16_t i)
{
	return msgpack_pack_uint16(&(caller->return_packer),i);
}

static inline int badger_return_uint32(badger_caller_t *caller,uint32_t i)
{
	return msgpack_pack_uint32(&(caller->return_packer),i);
}

static inline int badger_return_uint64(badger_caller_t *caller,uint64_t i)
{
	return msgpack_pack_uint64(&(caller->return_packer),i);
}

static inline int badger_return_int(badger_caller_t *caller,int i)
{
	return msgpack_pack_int(&(caller->return_packer),i);
}

static inline int badger_return_int8(badger_caller_t *caller,int8_t i)
{
	return msgpack_pack_int8(&(caller->return_packer),i);
}

static inline int badger_return_int16(badger_caller_t *caller,int16_t i)
{
	return msgpack_pack_int16(&(caller->return_packer),i);
}

static inline int badger_return_int32(badger_caller_t *caller,int32_t i)
{
	return msgpack_pack_int32(&(caller->return_packer),i);
}

static inline int badger_return_int64(badger_caller_t *caller,int64_t i)
{
	return msgpack_pack_int64(&(caller->return_packer),i);
}

static inline int badger_return_double(badger_caller_t *caller,double d)
{
	return msgpack_pack_double(&(caller->return_packer),d);
}

static inline int badger_return_raw(badger_caller_t *caller,const void *data,size_t length)
{
	msgpack_pack_raw(&(caller->return_packer),length);

	return msgpack_pack_raw_body(&(caller->return_packer),data,length);
}

static inline int badger_return_string(badger_caller_t *caller,const char *str)
{
	return badger_return_raw(caller,str,strlen(str));
}

static inline int badger_return_array(badger_caller_t *caller,unsigned int length)
{
	return msgpack_pack_array(&(caller->return_packer),length);
}

static inline int badger_return_map(badger_caller_t *caller,unsigned int length)
{
	return msgpack_pack_map(&(caller->return_packer),length);
}

static inline int badger_yield_nil(badger_caller_t *caller)
{
	return msgpack_pack_nil(&(caller->yield_packer));
}

static inline int badger_yield_boolean(badger_caller_t *caller,unsigned int value)
{
	if (value)
	{
		return msgpack_pack_true(&(caller->yield_packer));
	}
	else
	{
		return msgpack_pack_false(&(caller->yield_packer));
	}
}

static inline int badger_yield_true(badger_caller_t *caller)
{
	return msgpack_pack_true(&(caller->yield_packer));
}

static inline int badger_yield_false(badger_caller_t *caller)
{
	return msgpack_pack_false(&(caller->yield_packer));
}

static inline int badger_yield_uint(badger_caller_t *caller,unsigned int i)
{
	return msgpack_pack_unsigned_int(&(caller->yield_packer),i);
}

static inline int badger_yield_uint8(badger_caller_t *caller,uint8_t i)
{
	return msgpack_pack_uint8(&(caller->yield_packer),i);
}

static inline int badger_yield_uint16(badger_caller_t *caller,uint16_t i)
{
	return msgpack_pack_uint16(&(caller->yield_packer),i);
}

static inline int badger_yield_uint32(badger_caller_t *caller,uint32_t i)
{
	return msgpack_pack_uint32(&(caller->yield_packer),i);
}

static inline int badger_yield_uint64(badger_caller_t *caller,uint64_t i)
{
	return msgpack_pack_uint64(&(caller->yield_packer),i);
}

static inline int badger_yield_int(badger_caller_t *caller,int i)
{
	return msgpack_pack_int(&(caller->yield_packer),i);
}

static inline int badger_yield_int8(badger_caller_t *caller,int8_t i)
{
	return msgpack_pack_int8(&(caller->yield_packer),i);
}

static inline int badger_yield_int16(badger_caller_t *caller,int16_t i)
{
	return msgpack_pack_int16(&(caller->yield_packer),i);
}

static inline int badger_yield_int32(badger_caller_t *caller,int32_t i)
{
	return msgpack_pack_int32(&(caller->yield_packer),i);
}

static inline int badger_yield_int64(badger_caller_t *caller,int64_t i)
{
	return msgpack_pack_int64(&(caller->yield_packer),i);
}

static inline int badger_yield_double(badger_caller_t *caller,double d)
{
	return msgpack_pack_double(&(caller->yield_packer),d);
}

static inline int badger_yield_raw(badger_caller_t *caller,const void *data,size_t length)
{
	msgpack_pack_raw(&(caller->yield_packer),length);

	return msgpack_pack_raw_body(&(caller->yield_packer),data,length);
}

static inline int badger_yield_string(badger_caller_t *caller,const char *str)
{
	return badger_yield_raw(caller,str,strlen(str));
}

static inline int badger_yield_array(badger_caller_t *caller,unsigned int length)
{
	return msgpack_pack_array(&(caller->yield_packer),length);
}

static inline int badger_yield_map(badger_caller_t *caller,unsigned int length)
{
	return msgpack_pack_map(&(caller->yield_packer),length);
}

extern void badger_yield(badger_caller_t *caller);

#endif
