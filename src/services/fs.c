#include <badger/services/fs.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

badger_service_t badger_fs_service = {
	"fs",
	{
		&badger_fs_open_func,
		&badger_fs_close_func,
		NULL
	}
};

const badger_function_t badger_fs_open_func = {"open",badger_fs_open,1,{badger_data_string}};

int badger_fs_open(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	size_t path_length = badger_string_length(args);
	char path[path_length + 1];

	memcpy(path,badger_string(args),path_length);
	path[path_length] = '\0';

	int fd;

	if ((fd = open(path,O_RDWR)) == -1)
	{
		return BADGER_ERROR;
	}

	badger_return_int(caller,fd);
	return BADGER_SUCCESS;
}

const badger_function_t badger_fs_close_func = {"close",badger_fs_close,1,{badger_data_int}};

int badger_fs_close(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	if (close(badger_int(args) == -1))
	{
		return BADGER_ERROR;
	}

	return BADGER_SUCCESS;
}
