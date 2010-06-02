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

#ifndef _BADGER_SERVICES_SYS_H_
#define _BADGER_SERVICES_SYS_H_

#include <badger/service.h>

extern badger_service_t badger_sys_service;

extern const badger_function_t badger_sys_time_func;
extern int badger_sys_time(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_sys_chdir_func;
extern int badger_sys_chdir(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_sys_getcwd_func;
extern int badger_sys_getcwd(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_sys_gethostid_func;
extern int badger_sys_gethostid(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_sys_gethostname_func;
extern int badger_sys_gethostname(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_sys_getdomainname_func;
extern int badger_sys_getdomainname(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_sys_getpid_func;
extern int badger_sys_getpid(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_sys_getuid_func;
extern int badger_sys_getuid(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_sys_geteuid_func;
extern int badger_sys_geteuid(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_sys_getgid_func;
extern int badger_sys_getgid(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_sys_getegid_func;
int badger_sys_getegid(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_sys_getlogin_func;
extern int badger_sys_getlogin(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_sys_getusershell_func;
extern int badger_sys_getusershell(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_sys_popen_func;
int badger_sys_popen(int argc,const badger_data_t *args,badger_caller_t *caller);

#endif
