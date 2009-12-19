#ifndef _RONIN_RAT_LIBS_H_
#define _RONIN_RAT_LIBS_H_

#include <ronin/rat/tree.h>

typedef ronin_rat_tree_t ronin_rat_libs_t;

extern ronin_rat_libs_t * ronin_rat_libs_create();
extern int ronin_rat_libs_add(ronin_rat_libs_t *libs,ronin_rat_lib_t *lib);
extern int ronin_rat_libs_load(ronin_rat_libs_t *libs,const char *lib);
extern ronin_rat_lib_t * ronin_rat_libs_search(ronin_rat_libs_t *libs,const char *lib);
extern ronin_rat_func_t * ronin_rat_libs_lookup(ronin_rat_libs_t *libs,const char *lib,const char *func);
extern void ronin_rat_libs_destroy(ronin_rat_libs_t *libs);

#endif
