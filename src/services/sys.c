#include <badger/services/sys.h>

#include <time.h>
#include <unistd.h>

badger_service_t badger_sys_service = {
	"sys",
	{
		&badger_sys_time_func,
		&badger_sys_chdir_func,
		&badger_sys_getcwd_func,
		&badger_sys_gethostname_func,
		NULL
	}
};

const badger_func_t badger_sys_time_func = {"time",badger_sys_time,0};

int badger_sys_time(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	badger_return_uint(caller,time(NULL));
	return BADGER_SUCCESS;
}

const badger_func_t badger_sys_chdir_func = {"chdir",badger_sys_chdir,1,{badger_data_string}};

int badger_sys_chdir(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	if (chdir(badger_string(args)) == -1)
	{
		return BADGER_ERROR;
	}

	return BADGER_SUCCESS;
}

const badger_func_t badger_sys_getcwd_func = {"getcwd",badger_sys_getcwd,0};

int badger_sys_getcwd(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	char cwd[PATH_MAX];

	if (!getcwd(cwd,PATH_MAX))
	{
		// copy failed
		return BADGER_ERROR;
	}

	badger_return_string(caller,cwd);
	return BADGER_SUCCESS;
}

const badger_func_t badger_sys_gethostname_func = {"gethostname",badger_sys_gethostname,0};

int badger_sys_gethostname(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	char hostname[HOST_NAME_MAX];

	if (gethostname(hostname,HOST_NAME_MAX) == -1)
	{
		return BADGER_ERROR;
	}

	badger_return_string(caller,hostname);
	return BADGER_SUCCESS;
}
