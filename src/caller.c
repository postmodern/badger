#include <badger/caller.h>
#include "private/caller.h"
#include "private/server.h"

#include <msgpack/sbuffer.h>

int badger_return_nil(badger_caller_t *caller)
{
	return msgpack_pack_nil(&(caller->ret.packer));
}

int badger_return_boolean(badger_caller_t *caller,unsigned int value)
{
	if (value)
	{
		return msgpack_pack_true(&(caller->ret.packer));
	}
	else
	{
		return msgpack_pack_false(&(caller->ret.packer));
	}
}

int badger_return_true(badger_caller_t *caller)
{
	return msgpack_pack_true(&(caller->ret.packer));
}

int badger_return_false(badger_caller_t *caller)
{
	return msgpack_pack_false(&(caller->ret.packer));
}

int badger_return_uint(badger_caller_t *caller,unsigned int i)
{
	return msgpack_pack_unsigned_int(&(caller->ret.packer),i);
}

int badger_return_uint8(badger_caller_t *caller,uint8_t i)
{
	return msgpack_pack_uint8(&(caller->ret.packer),i);
}

int badger_return_uint16(badger_caller_t *caller,uint16_t i)
{
	return msgpack_pack_uint16(&(caller->ret.packer),i);
}

int badger_return_uint32(badger_caller_t *caller,uint32_t i)
{
	return msgpack_pack_uint32(&(caller->ret.packer),i);
}

int badger_return_uint64(badger_caller_t *caller,uint64_t i)
{
	return msgpack_pack_uint64(&(caller->ret.packer),i);
}

int badger_return_int(badger_caller_t *caller,int i)
{
	return msgpack_pack_int(&(caller->ret.packer),i);
}

int badger_return_int8(badger_caller_t *caller,int8_t i)
{
	return msgpack_pack_int8(&(caller->ret.packer),i);
}

int badger_return_int16(badger_caller_t *caller,int16_t i)
{
	return msgpack_pack_int16(&(caller->ret.packer),i);
}

int badger_return_int32(badger_caller_t *caller,int32_t i)
{
	return msgpack_pack_int32(&(caller->ret.packer),i);
}

int badger_return_int64(badger_caller_t *caller,int64_t i)
{
	return msgpack_pack_int64(&(caller->ret.packer),i);
}

int badger_return_double(badger_caller_t *caller,double d)
{
	return msgpack_pack_double(&(caller->ret.packer),d);
}

int badger_return_raw(badger_caller_t *caller,const void *data,size_t length)
{
	msgpack_pack_raw(&(caller->ret.packer),length);

	return msgpack_pack_raw_body(&(caller->ret.packer),data,length);
}

int badger_return_string(badger_caller_t *caller,const char *str)
{
	return badger_return_raw(caller,str,strlen(str) + 1);
}

int badger_return_array(badger_caller_t *caller,unsigned int length)
{
	return msgpack_pack_array(&(caller->ret.packer),length);
}

int badger_return_map(badger_caller_t *caller,unsigned int length)
{
	return msgpack_pack_map(&(caller->ret.packer),length);
}

int badger_yield_nil(badger_caller_t *caller)
{
	return msgpack_pack_nil(&(caller->yield.packer));
}

int badger_yield_boolean(badger_caller_t *caller,unsigned int value)
{
	if (value)
	{
		return msgpack_pack_true(&(caller->yield.packer));
	}
	else
	{
		return msgpack_pack_false(&(caller->yield.packer));
	}
}

int badger_yield_true(badger_caller_t *caller)
{
	return msgpack_pack_true(&(caller->yield.packer));
}

int badger_yield_false(badger_caller_t *caller)
{
	return msgpack_pack_false(&(caller->yield.packer));
}

int badger_yield_uint(badger_caller_t *caller,unsigned int i)
{
	return msgpack_pack_unsigned_int(&(caller->yield.packer),i);
}

int badger_yield_uint8(badger_caller_t *caller,uint8_t i)
{
	return msgpack_pack_uint8(&(caller->yield.packer),i);
}

int badger_yield_uint16(badger_caller_t *caller,uint16_t i)
{
	return msgpack_pack_uint16(&(caller->yield.packer),i);
}

int badger_yield_uint32(badger_caller_t *caller,uint32_t i)
{
	return msgpack_pack_uint32(&(caller->yield.packer),i);
}

int badger_yield_uint64(badger_caller_t *caller,uint64_t i)
{
	return msgpack_pack_uint64(&(caller->yield.packer),i);
}

int badger_yield_int(badger_caller_t *caller,int i)
{
	return msgpack_pack_int(&(caller->yield.packer),i);
}

int badger_yield_int8(badger_caller_t *caller,int8_t i)
{
	return msgpack_pack_int8(&(caller->yield.packer),i);
}

int badger_yield_int16(badger_caller_t *caller,int16_t i)
{
	return msgpack_pack_int16(&(caller->yield.packer),i);
}

int badger_yield_int32(badger_caller_t *caller,int32_t i)
{
	return msgpack_pack_int32(&(caller->yield.packer),i);
}

int badger_yield_int64(badger_caller_t *caller,int64_t i)
{
	return msgpack_pack_int64(&(caller->yield.packer),i);
}

int badger_yield_double(badger_caller_t *caller,double d)
{
	return msgpack_pack_double(&(caller->yield.packer),d);
}

int badger_yield_raw(badger_caller_t *caller,const void *data,size_t length)
{
	msgpack_pack_raw(&(caller->yield.packer),length);

	return msgpack_pack_raw_body(&(caller->yield.packer),data,length);
}

int badger_yield_string(badger_caller_t *caller,const char *str)
{
	return badger_yield_raw(caller,str,strlen(str) + 1);
}

int badger_yield_array(badger_caller_t *caller,unsigned int length)
{
	return msgpack_pack_array(&(caller->yield.packer),length);
}

int badger_yield_map(badger_caller_t *caller,unsigned int length)
{
	return msgpack_pack_map(&(caller->yield.packer),length);
}

int badger_yield(badger_caller_t *caller)
{
	int ret;

	// pack, encode and push the yield data
	ret = badger_server_pack(caller->server,caller->yield.buffer.data,caller->yield.buffer.size);

	// reset the yield buffer
	badger_response_reset(&(caller->yield),BADGER_RESPONSE_YIELD);
	return ret;
}
