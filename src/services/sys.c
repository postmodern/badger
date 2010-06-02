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

#include <badger/services/sys.h>

#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

badger_service_t badger_sys_service = {
	"sys",
	{
		&badger_sys_time_func,
		&badger_sys_chdir_func,
		&badger_sys_getcwd_func,
		&badger_sys_gethostid_func,
		&badger_sys_gethostname_func,
		&badger_sys_getdomainname_func,
		&badger_sys_getpid_func,
		&badger_sys_getuid_func,
		&badger_sys_geteuid_func,
		&badger_sys_getgid_func,
		&badger_sys_getegid_func,
		&badger_sys_getlogin_func,
		&badger_sys_getusershell_func,
		&badger_sys_popen_func,
		NULL
	}
};

const badger_function_t badger_sys_time_func = {"time",badger_sys_time,badger_data_nil,0};

int badger_sys_time(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	badger_return_uint(caller,time(NULL));
	return BADGER_SUCCESS;
}

const badger_function_t badger_sys_chdir_func = {"chdir",badger_sys_chdir,badger_data_nil,1,{badger_data_string}};

int badger_sys_chdir(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	if (chdir(badger_string(args)) == -1)
	{
		badger_return_errno(caller);
		return BADGER_ERROR;
	}

	return BADGER_SUCCESS;
}

const badger_function_t badger_sys_getcwd_func = {"getcwd",badger_sys_getcwd,badger_data_string,0};

int badger_sys_getcwd(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	char cwd[PATH_MAX];

	if (!getcwd(cwd,PATH_MAX))
	{
		// copy failed
		badger_return_errno(caller);
		return BADGER_ERROR;
	}

	badger_return_string(caller,cwd);
	return BADGER_SUCCESS;
}

const badger_function_t badger_sys_gethostid_func = {"gethostid",badger_sys_gethostid,badger_data_uint,0};

int badger_sys_gethostid(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	badger_return_uint(caller,gethostid());
	return BADGER_SUCCESS;
}

const badger_function_t badger_sys_gethostname_func = {"gethostname",badger_sys_gethostname,badger_data_string,0};

int badger_sys_gethostname(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	char hostname[HOST_NAME_MAX];

	if (gethostname(hostname,HOST_NAME_MAX) == -1)
	{
		badger_return_errno(caller);
		return BADGER_ERROR;
	}

	badger_return_string(caller,hostname);
	return BADGER_SUCCESS;
}

const badger_function_t badger_sys_getdomainname_func = {"getdomainname",badger_sys_getdomainname,badger_data_string,0};

int badger_sys_getdomainname(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	char domainname[64];

	if (getdomainname(domainname,64) == -1)
	{
		badger_return_errno(caller);
		return BADGER_ERROR;
	}

	badger_return_string(caller,domainname);
	return BADGER_SUCCESS;
}

const badger_function_t badger_sys_getpid_func = {"getpid",badger_sys_getpid,badger_data_uint,0};

int badger_sys_getpid(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	badger_return_uint(caller,getpid());
	return BADGER_SUCCESS;
}

const badger_function_t badger_sys_getuid_func = {"getuid",badger_sys_getuid,badger_data_uint,0};

int badger_sys_getuid(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	badger_return_uint(caller,getuid());
	return BADGER_SUCCESS;
}

const badger_function_t badger_sys_geteuid_func = {"geteuid",badger_sys_geteuid,badger_data_uint,0};

int badger_sys_geteuid(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	badger_return_uint(caller,geteuid());
	return BADGER_SUCCESS;
}

const badger_function_t badger_sys_getgid_func = {"getgid",badger_sys_getgid,badger_data_uint,0};

int badger_sys_getgid(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	badger_return_uint(caller,getgid());
	return BADGER_SUCCESS;
}

const badger_function_t badger_sys_getegid_func = {"getegid",badger_sys_getegid,badger_data_uint,0};

int badger_sys_getegid(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	badger_return_uint(caller,getegid());
	return BADGER_SUCCESS;
}

const badger_function_t badger_sys_getlogin_func = {"getlogin",badger_sys_getlogin,badger_data_string,0};

int badger_sys_getlogin(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	char login[256];

	if (getlogin_r(login,256) != 0)
	{
		badger_return_errno(caller);
		return BADGER_ERROR;
	}

	badger_return_string(caller,login);
	return BADGER_SUCCESS;
}

const badger_function_t badger_sys_getusershell_func = {"getusershell",badger_sys_getusershell,badger_data_string,0};

int badger_sys_getusershell(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	badger_return_string(caller,getusershell());
	return BADGER_SUCCESS;
}

const badger_function_t badger_sys_popen_func = {"popen",badger_sys_popen,badger_data_int,1,{badger_data_string}};

int badger_sys_popen(int argc,const badger_data_t *args,badger_caller_t *caller)
{
	size_t command_length = badger_string_length(args);
	char command[command_length + 1];

	memcpy(command,badger_string(args),command_length);
	command[command_length] = '\0';

	FILE *proc;

	if (!(proc = popen(badger_string(args),"r")))
	{
		badger_return_errno(caller);
		return BADGER_ERROR;
	}

	unsigned char buffer[1024];
	size_t length;

	while (!feof(proc))
	{
		if ((length = fread(buffer,1,1024,proc)))
		{
			badger_yield_open(caller,1);
			badger_yield_raw(caller,buffer,length);
			badger_yield_close(caller);
		}
	}

	int status;

	if ((status = pclose(proc)) == -1)
	{
		badger_return_errno(caller);
		return BADGER_ERROR;
	}

	badger_return_int(caller,status);
	return BADGER_SUCCESS;
}
