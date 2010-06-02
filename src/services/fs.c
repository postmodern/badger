/*
 * Badger - A fear-less, robust and portable systems management solution.
 *
 * Copyright (c) 2010 Hal Brodigan (postmodern.mod3 at gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#include <badger/services/fs.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

badger_service_t badger_fs_service = {
	"fs",
	{
		&badger_fs_stat_func,
		&badger_fs_unlink_func,
		&badger_fs_open_func,
		&badger_fs_seek_func,
		&badger_fs_read_func,
		&badger_fs_write_func,
		&badger_fs_close_func,
		NULL
	}
};

const badger_function_t badger_fs_stat_func = {"stat",badger_fs_stat,badger_data_array,1,{badger_data_string}};

int badger_fs_stat(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	size_t path_length = badger_string_length(args);
	char path[path_length + 1];

	memcpy(path,badger_string(args),path_length);
	path[path_length] = '\0';

	struct stat file;

	if (stat(path,&file) == -1)
	{
		badger_return_errno(caller);
		return BADGER_ERROR;
	}

	badger_return_array(caller,7);

	// protection
	badger_return_uint(caller,file.st_mode);
	// user ID of owner
	badger_return_uint(caller,file.st_uid);
	// group ID of owner
	badger_return_uint(caller,file.st_gid);
	// total size, in bytes
	badger_return_uint(caller,file.st_size);
	// time of last access
	badger_return_uint(caller,file.st_atime);
	// time of last modification
	badger_return_uint(caller,file.st_mtime);
	// time of last status change
	badger_return_uint(caller,file.st_ctime);
	return BADGER_SUCCESS;
}

const badger_function_t badger_fs_unlink_func = {"unlink",badger_fs_unlink,badger_data_nil,1,{badger_data_string}};

int badger_fs_unlink(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	size_t path_length = badger_string_length(args);
	char path[path_length + 1];

	memcpy(path,badger_string(args),path_length);
	path[path_length] = '\0';

	if (unlink(path) == -1)
	{
		badger_return_errno(caller);
		return BADGER_ERROR;
	}

	return BADGER_SUCCESS;
}

const badger_function_t badger_fs_open_func = {"open",badger_fs_open,badger_data_int,1,{badger_data_string}};

int badger_fs_open(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	size_t path_length = badger_string_length(args);
	char path[path_length + 1];

	memcpy(path,badger_string(args),path_length);
	path[path_length] = '\0';

	int fd;

	if ((fd = open(path,O_RDWR)) == -1)
	{
		badger_return_errno(caller);
		return BADGER_ERROR;
	}

	badger_return_int(caller,fd);
	return BADGER_SUCCESS;
}

const badger_function_t badger_fs_seek_func = {"seek",badger_fs_seek,badger_data_uint,3,{badger_data_int,badger_data_uint,badger_data_string}};

int badger_fs_seek(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	size_t whence_length = badger_string_length(args+2);
	const char *whence_name = badger_string(args+2);
	int whence = 0;

	if ((whence_length == 3) && (!memcmp(whence_name,"set",3)))
	{
		whence = SEEK_SET;
	}
	else if ((whence_length == 7) && (!memcmp(whence_name,"current",7)))
	{
		whence = SEEK_CUR;
	}
	else if ((whence_length == 3) && (!memcmp(whence_name,"end",3)))
	{
		whence = SEEK_END;
	}

	size_t offset;

	if ((offset = lseek(badger_int(args),badger_uint(args+1),whence)) == -1)
	{
		badger_return_errno(caller);
		return BADGER_ERROR;
	}

	badger_return_uint(caller,offset);
	return BADGER_SUCCESS;
}

const badger_function_t badger_fs_read_func = {"read",badger_fs_read,badger_data_raw,2,{badger_data_int,badger_data_uint}};

int badger_fs_read(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	size_t length = badger_uint(args+1);
	unsigned char buffer[length];
	size_t count;

	if ((count = read(badger_int(args),buffer,length)) == -1)
	{
		badger_return_errno(caller);
		return BADGER_ERROR;
	}

	badger_return_raw(caller,buffer,count);

	memset(buffer,0,length);
	return BADGER_SUCCESS;
}

const badger_function_t badger_fs_write_func = {"write",badger_fs_write,badger_data_int,2,{badger_data_int,badger_data_raw}};

int badger_fs_write(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	ssize_t count;

	if ((count = write(badger_int(args),badger_raw(args+2),badger_raw_length(args+2))) == -1)
	{
		badger_return_errno(caller);
		return BADGER_ERROR;
	}

	badger_return_int(caller,count);
	return BADGER_SUCCESS;
}

const badger_function_t badger_fs_close_func = {"close",badger_fs_close,badger_data_nil,1,{badger_data_int}};

int badger_fs_close(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	if (close(badger_int(args)) == -1)
	{
		badger_return_errno(caller);
		return BADGER_ERROR;
	}

	return BADGER_SUCCESS;
}
