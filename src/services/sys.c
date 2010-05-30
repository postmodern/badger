#include <badger/services/sys.h>

#include <time.h>
#include <unistd.h>

badger_service_t badger_sys_service = {
	"sys",
	{
		&badger_sys_time_func,
		&badger_sys_chdir_func,
		NULL
	}
};

const badger_func_t badger_sys_time_func = {"time",badger_sys_time,0};

int badger_sys_time(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	badger_return_uint(caller,time(NULL));
	return 0;
}

const badger_func_t badger_sys_chdir_func = {"chdir",badger_sys_chdir,1,{badger_data_string}};

int badger_sys_chdir(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	chdir(badger_string(args));
	return 0;
}
