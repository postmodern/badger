#ifndef _BADGER_FUNC_H_
#define _BADGER_FUNC_H_

#include <badger/data.h>

#include <sys/types.h>

typedef int (*badger_func_ptr)(int argc,const badger_data_t *args,badger_ret_t *ret);

struct badger_func
{
	const char *name;

	badger_func_ptr ptr;

	ssize_t argc;
	const badger_data_type arg_types[];
};
typedef struct badger_func badger_func_t;

extern int badger_func_valid(const badger_func_t *func,int argc,const badger_data_t *args);
extern int badger_func_call(const badger_func_t *func,int argc,const badger_data_t *args,badger_ret_t *ret);

#endif
