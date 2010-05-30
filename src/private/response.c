#include "private/response.h"

#include <string.h>

void badger_response_init(badger_response_t *response,badger_request_id id,badger_response_type type)
{
	response->id = id;
	response->type = type;

	msgpack_sbuffer_init(&(response->buffer));
	msgpack_packer_init(&(response->packer),&(response->buffer),msgpack_sbuffer_write);

	// all responses have two elements by default
	uint32_t array_length = 2;

	switch (type)
	{
		case BADGER_RESPONSE_PONG:
		case BADGER_RESPONSE_SERVICES:
		case BADGER_RESPONSE_FUNCTIONS:
		case BADGER_RESPONSE_YIELD:
		case BADGER_RESPONSE_RETURN:
			array_length += 1;
			break;
		case BADGER_RESPONSE_ERROR:
			array_length += 2;
			break;
		default:
			badger_debug("badger_response_init: unknown badger response type (%u)\n",type);
			assert(0);
	}

	// pack all responses inside an array
	msgpack_pack_array(&(response->packer),array_length);

	// packs the request-ID we are responding to
	msgpack_pack_uint32(&(response->packer),response->id);

	// packs the response type
	msgpack_pack_uint8(&(response->packer),type);
}

void badger_response_clear(badger_response_t *response)
{
	response->id = 0;
	response->type = 0;

	if (response->buffer.data)
	{
		// zero the buffer before freeing it
		memset(response->buffer.data,0,response->buffer.size);
	}
	msgpack_sbuffer_destroy(&(response->buffer));
}

void badger_response_reset(badger_response_t *response,badger_response_type type)
{
	badger_request_id id = response->id;

	badger_response_clear(response);
	badger_response_init(response,id,type);
}
