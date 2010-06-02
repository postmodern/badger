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

#ifndef _BADGER_SERVICES_FS_H_
#define _BADGER_SERVICES_FS_H_

#include <badger/service.h>

extern badger_service_t badger_fs_service;

extern const badger_function_t badger_fs_stat_func;
extern int badger_fs_stat(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_fs_unlink_func;
extern int badger_fs_unlink(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_fs_open_func;
extern int badger_fs_open(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_fs_seek_func;
extern int badger_fs_seek(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_fs_read_func;
extern int badger_fs_read(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_fs_write_func;
extern int badger_fs_write(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_fs_close_func;
extern int badger_fs_close(int argc,const badger_data_t *args,badger_caller_t *caller);

#endif
