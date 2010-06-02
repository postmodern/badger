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

#include "private/ffi/value.h"
#include "private/util.h"
#include <badger/errno.h>

#include <malloc.h>
#include <string.h>

void ffi_value_clear(ffi_value_t *value)
{
	memset(value,0,sizeof(ffi_value_t));
}

int badger_ffi_value_init(ffi_value_t *value,const ffi_type *type,const badger_data_t *data)
{
	unsigned char *raw_ptr;
	size_t raw_length;

	switch (type->type)
	{
		case FFI_TYPE_INT:
			switch (badger_type(data))
			{
				case badger_data_int:
					value->ui = badger_int(data);
					break;
				case badger_data_uint:
					value->ui = badger_uint(data);
					break;
				default:
					goto incompatible_type;
			}
			break;
		case FFI_TYPE_UINT8:
			switch (badger_type(data))
			{
				case badger_data_int:
					value->u8 = badger_int(data);
					break;
				case badger_data_uint:
					value->u8 = badger_uint(data);
					break;
				default:
					goto incompatible_type;
			}
			break;
		case FFI_TYPE_UINT16:
			switch (badger_type(data))
			{
				case badger_data_int:
					value->u16 = badger_int(data);
					break;
				case badger_data_uint:
					value->u16 = badger_uint(data);
					break;
				default:
					goto incompatible_type;
			}
			break;
		case FFI_TYPE_UINT32:
			switch (badger_type(data))
			{
				case badger_data_int:
					value->u32 = badger_int(data);
					break;
				case badger_data_uint:
					value->u32 = badger_uint(data);
					break;
				default:
					goto incompatible_type;
			}
			break;
		case FFI_TYPE_UINT64:
			switch (badger_type(data))
			{
				case badger_data_int:
					value->u64 = badger_int(data);
					break;
				case badger_data_uint:
					value->u64 = badger_uint(data);
					break;
				default:
					goto incompatible_type;
			}
			break;
		case FFI_TYPE_SINT8:
			switch (badger_type(data))
			{
				case badger_data_int:
					value->s8 = badger_int(data);
					break;
				case badger_data_uint:
					value->s8 = badger_uint(data);
					break;
				default:
					goto incompatible_type;
			}
			break;
		case FFI_TYPE_SINT16:
			switch (badger_type(data))
			{
				case badger_data_int:
					value->s16 = badger_int(data);
					break;
				case badger_data_uint:
					value->s16 = badger_uint(data);
					break;
				default:
					goto incompatible_type;
			}
			break;
		case FFI_TYPE_SINT32:
			switch (badger_type(data))
			{
				case badger_data_int:
					value->s32 = badger_int(data);
					break;
				case badger_data_uint:
					value->s32 = badger_uint(data);
					break;
				default:
					goto incompatible_type;
			}
			break;
		case FFI_TYPE_SINT64:
			switch (badger_type(data))
			{
				case badger_data_int:
					value->s64 = badger_int(data);
					break;
				case badger_data_uint:
					value->s64 = badger_uint(data);
					break;
				default:
					goto incompatible_type;
			}
			break;
		case FFI_TYPE_FLOAT:
			switch (badger_type(data))
			{
				case badger_data_float:
					value->f = badger_float(data);
					break;
				default:
					goto incompatible_type;
			}
			break;
		case FFI_TYPE_DOUBLE:
		case FFI_TYPE_LONGDOUBLE:
			switch (badger_type(data))
			{
				case badger_data_float:
					value->fl = badger_float(data);
					break;
				default:
					goto incompatible_type;
			}
			break;
		case FFI_TYPE_POINTER:
			switch (badger_type(data))
			{
				case badger_data_uint:
					value->ptr = (void *)badger_uint(data);
					break;
				case badger_data_raw:
					raw_length = badger_raw_length(data);

					if (!(raw_ptr = malloc(raw_length + 1)))
					{
						// malloc failed
						goto error;
					}

					// copy the raw buffer and append a \0 byte to it (just in case)
					memcpy(raw_ptr,badger_raw(data),raw_length);
					raw_ptr[raw_length] = '\0';

					value->ptr = raw_ptr;
					break;
				default:
					goto incompatible_type;
			}
			break;
		default:
			// unsupported FFI type
			goto unknown_type;
	}

	return BADGER_SUCCESS;

incompatible_type:
	badger_debug("badger_ffi_value_init: incompatible data type for FFI %s\n",ffi_types_name(type));
	return BADGER_ERRNO_ARG_TYPE;

unknown_type:
	badger_debug("badger_ffi_invoke: unknown FFI type (%u)\n",type->type);
error:
	return BADGER_ERROR;
}

void badger_ffi_value_fini(ffi_value_t *value,const ffi_type *type,const badger_data_t *data)
{
	switch (type->type)
	{
		case FFI_TYPE_POINTER:
			switch (badger_type(data))
			{
				case badger_data_raw:
					// zero the copied raw buffer before freeing it
					memset(value->ptr,0,badger_raw_length(data));
					free(value->ptr);
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}

	ffi_value_clear(value);
}
