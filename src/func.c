#include <badger/func.h>
#include <badger/errno.h>

#include <stdlib.h>
#include <string.h>

int badger_func_call(badger_func_t *func,int argc,msgpack_object *args,msgpack_object **ret)
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
