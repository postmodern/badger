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

#include <badger/data.h>

const char * badger_data_type_name(badger_data_type type)
{
	switch (type)
	{
		case badger_data_nil:
			return "nil";
		case badger_data_boolean:
			return "boolean";
		case badger_data_uint:
			return "uint";
		case badger_data_int:
			return "int";
		case badger_data_double:
			return "double";
		case badger_data_string:
			return "string";
		case badger_data_array:
			return "array";
		case badger_data_map:
			return "map";
		case badger_data_any:
			return "any";
		default:
			return "unknown";
	}
}
