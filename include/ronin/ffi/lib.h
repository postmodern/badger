#ifndef _RONIN_RAT_LIB_H_
#define _RONIN_RAT_LIB_H_

#include <ronin/rat/tree.h>
#include <ronin/rat/func.h>

struct ronin_rat_lib
{
	char *name;
	void *handler;

	ronin_rat_tree_t *funcs;
};
typedef struct ronin_rat_lib ronin_rat_lib_t;

extern ronin_rat_lib_t * ronin_rat_lib_create();
extern ronin_rat_lib_t * ronin_rat_lib_load(const char *name);
extern ronin_rat_func_t * ronin_rat_lib_lookup(const ronin_rat_lib_t *lib,const char *name);
extern int ronin_rat_lib_close(ronin_rat_lib_t *lib);
extern void ronin_rat_lib_destroy(ronin_rat_lib_t *lib);

#endif
