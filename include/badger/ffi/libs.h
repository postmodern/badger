#ifndef _BADGER_LIBS_H_
#define _BADGER_LIBS_H_

#include <badger/slist.h>

typedef badger_slist_t badger_libs_t;

extern badger_libs_t * badger_libs_create();
extern int badger_libs_add(badger_libs_t *libs,badger_lib_t *lib);
extern int badger_libs_load(badger_libs_t *libs,const char *lib);
extern badger_lib_t * badger_libs_search(badger_libs_t *libs,const char *lib);
extern badger_func_t * badger_libs_lookup(badger_libs_t *libs,const char *lib,const char *func);
extern void badger_libs_destroy(badger_libs_t *libs);

#endif
