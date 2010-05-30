#include <badger/services/sys.h>

#include <time.h>

badger_service_t badger_sys_service = {
	"sys",
	{
		{"time",badger_sys_time,0},
		{NULL,NULL,0}
	}
};

int badger_sys_time(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	badger_return_uint(caller,time(NULL));
	return 0;
}
