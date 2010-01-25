#ifndef _BADGER_LIB_H_
#define _BADGER_LIB_H_

#include <badger/func.h>
#include <badger/slist.h>

struct badger_lib
{
	char *name;
	void *handler;

	badger_slist_t *funcs;
};
typedef struct badger_lib badger_lib_t;

extern badger_lib_t * badger_lib_create();
extern badger_lib_t * badger_lib_load(const char *name);
extern badger_func_t * badger_lib_lookup(const badger_lib_t *lib,const char *name);
extern int badger_lib_close(badger_lib_t *lib);
extern void badger_lib_destroy(badger_lib_t *lib);

#endif
