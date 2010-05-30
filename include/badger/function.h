#ifndef _BADGER_FUNCTION_H_
#define _BADGER_FUNCTION_H_

#include <badger/data.h>
#include <badger/caller.h>

#include <sys/types.h>

typedef int (*badger_function_ptr)(int argc,const badger_data_t *args,badger_caller_t *caller);

struct badger_function
{
	const char *name;

	badger_function_ptr ptr;

	ssize_t argc;
	const badger_data_type arg_types[];
};
typedef struct badger_function badger_function_t;

extern int badger_function_valid(const badger_function_t *function,int argc,const badger_data_t *args);
extern int badger_function_call(const badger_function_t *function,int argc,const badger_data_t *args,badger_caller_t *caller);

#endif
