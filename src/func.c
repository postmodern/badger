#include <badger/func.h>
#include <badger/errno.h>

#include <stdlib.h>
#include <string.h>

badger_func_t * badger_func_create(const char *name,badger_func_ptr ptr,int argc,bert_data_t *arg_types)
{
	badger_func_t *new_func;

	if (!(new_func = malloc(sizeof(badger_func_t))))
	{
		// malloc failed
		goto cleanup;
	}

	if (!(new_func->name = strdup(name)))
	{
		goto cleanup_func;
	}

	new_func->argc = argc;
	new_func->arg_types = NULL;

	if (argc > 0)
	{
		if (!(new_func->arg_types = malloc(sizeof(bert_data_type)*argc)))
		{
			goto cleanup_name;
		}
	}

	new_func->ptr = ptr;
	return new_func;

cleanup_name:
	free(new_func->name);
cleanup_func:
	free(new_func);
cleanup:
	return NULL;
}

int badger_func_call(badger_func_t *func,int argc,bert_data_t *args,bert_data_t **ret)
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

void badger_func_destroy(badger_func_t *func)
{
	if (func->argc > 0)
	{
		free(func->arg_types);
	}

	free(func->name);
	free(func);
}
