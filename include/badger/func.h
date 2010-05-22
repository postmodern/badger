#ifndef _BADGER_FUNC_H_
#define _BADGER_FUNC_H_

#include <msgpack/object.h>
#include <sys/types.h>

typedef int (*badger_func_ptr)(int argc,msgpack_object *args,msgpack_object **ret);

struct badger_func
{
	char *name;

	ssize_t argc;
	msgpack_object_type *arg_types;

	badger_func_ptr ptr;
};
typedef struct badger_func badger_func_t;

badger_func_t * badger_func_create(const char *name,badger_func_ptr ptr,int argc,msgpack_object_type *arg_types);
int badger_func_call(badger_func_t *func,int argc,msgpack_object *args,msgpack_object **ret);
void badger_func_destroy(badger_func_t *func);

#endif
