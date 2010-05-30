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
