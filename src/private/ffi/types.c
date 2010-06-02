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

#include "private/ffi/types.h"

#include <string.h>

const struct ffi_types_mapping ffi_types_map[] = {
	{"uint", &ffi_type_uint},
	{"uchar", &ffi_type_uchar},
	{"ushort", &ffi_type_ushort},
	{"ulong", &ffi_type_ulong},
	{"uint8", &ffi_type_uint8},
	{"uint16", &ffi_type_uint16},
	{"uint32", &ffi_type_uint32},
	{"uint64", &ffi_type_uint64},
	{"int", &ffi_type_sint},
	{"char", &ffi_type_schar},
	{"short", &ffi_type_sshort},
	{"long", &ffi_type_slong},
	{"int8", &ffi_type_sint8},
	{"int16", &ffi_type_sint16},
	{"int32", &ffi_type_sint32},
	{"int64", &ffi_type_sint64},
	{"float", &ffi_type_float},
	{"double", &ffi_type_double},
	{"pointer", &ffi_type_pointer},
	{"string", &ffi_type_pointer},
	{"void", &ffi_type_void},
	{NULL, NULL}
};

const ffi_type * ffi_types_parse(const char *name)
{
	unsigned int i = 0;

	while (ffi_types_map[i].name)
	{
		if (!strcmp(name,ffi_types_map[i].name))
		{
			return ffi_types_map[i].type;
		}

		++i;
	}

	return NULL;
}

const char * ffi_types_name(const ffi_type *type)
{
	unsigned int i = 0;

	while (ffi_types_map[i].name)
	{
		if (type == ffi_types_map[i].type)
		{
			return ffi_types_map[i].name;
		}

		++i;
	}

	return "unknown";
}
