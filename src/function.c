#include <badger/function.h>
#include <badger/errno.h>

int badger_function_valid(const badger_function_t *function,int argc,const badger_data_t *args)
{
	if (function->argc != -1)
	{
		if (argc != function->argc)
		{
			return BADGER_ERRNO_ARGC;
		}

		unsigned int i;

		for (i=0;i<argc;i++)
		{
			if (function->arg_types[i] != badger_data_any)
			{
				if (args[i].type != function->arg_types[i])
				{
					return BADGER_ERRNO_ARG_TYPE;
				}
			}
		}
	}

	return 0;
}

int badger_function_call(const badger_function_t *function,int argc,const badger_data_t *args,badger_caller_t *caller)
{
	return function->ptr(argc,args,caller);
}
