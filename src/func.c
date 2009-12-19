#include <ronin/rat/func.h>
#include <ronin/rat/errno.h>

#include <malloc.h>
#include <string.h>

ronin_rat_func_t * ronin_rat_func_create(const char *name,ronin_rat_func_ptr ptr,int argc,bert_data_t *arg_types)
{
	ronin_rat_func_t *new_func;

	if (!(new_func = malloc(sizeof(ronin_rat_func_t))))
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

int ronin_rat_func_call(ronin_rat_func_t *func,int argc,bert_data_t *args,bert_data_t **ret)
{
	if (func->argc != -1)
	{
		if (argc != func->argc)
		{
			return RONIN_RAT_ERRNO_ARGC;
		}

		unsigned int i;

		for (i=0;i<argc;i++)
		{
			if (args[i].type != func->arg_types[i])
			{
				return RONIN_RAT_ERRNO_ARG_TYPE;
			}
		}
	}

	return func->ptr(argc,args,ret);
}

void ronin_rat_func_destroy(ronin_rat_func_t *func)
{
	if (func->argc > 0)
	{
		free(func->arg_types);
	}

	free(func->name);
	free(func);
}
