#ifndef _BADGER_FUNC_H_
#define _BADGER_FUNC_H_

#include <msgpack/object.h>
#include <sys/types.h>

typedef int (*badger_func_ptr)(int argc,msgpack_object *args,msgpack_object **ret);

struct badger_func
{
	const char *name;

	badger_func_ptr ptr;

	ssize_t argc;
	const msgpack_object_type arg_types[];
};
typedef struct badger_func badger_func_t;

extern int badger_func_call(badger_func_t *func,int argc,msgpack_object *args,msgpack_object **ret);

#endif
