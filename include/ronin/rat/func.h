#ifndef _RONIN_RAT_FUNC_H_
#define _RONIN_RAT_FUNC_H_

#include <bert/data.h>
#include <sys/types.h>

typedef int (*ronin_rat_func_ptr)(int argc,bert_data_t *args);

struct ronin_rat_func
{
	char *name;

	ssize_t argc;
	bert_data_type *arg_types;

	ronin_rat_func_ptr address;
};
typedef struct ronin_rat_func ronin_rat_func_t;

ronin_rat_func_t * ronin_rat_func_create(const char *name,ronin_rat_func_ptr address,int argc,bert_data_t *arg_types);
void ronin_rat_func_destroy(ronin_rat_func_t *func);

#endif
