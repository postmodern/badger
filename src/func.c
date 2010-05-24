#include <badger/func.h>
#include <badger/errno.h>

int badger_func_call(const badger_func_t *func,int argc,const badger_data_t *args,badger_ret_t *ret)
{
	if (func->argc != -1)
	{
		if (argc != func->argc)
		{
			return BADGER_ERRNO_ARGC;
		}

		unsigned int i;

		for (i=0;i<argc;i++)
		{
			if (args[i].type != func->arg_types[i])
			{
				return BADGER_ERRNO_ARG_TYPE;
			}
		}
	}

	return func->ptr(argc,args,ret);
}
