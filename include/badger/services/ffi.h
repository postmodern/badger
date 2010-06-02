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

#ifndef _BADGER_SERVICES_FFI_H_
#define _BADGER_SERVICES_FFI_H_

#include <badger/service.h>

extern const badger_service_t badger_ffi_service;

extern int badger_ffi_init();

extern const badger_function_t badger_ffi_open_func;
extern int badger_ffi_open(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_ffi_libraries_func;
extern int badger_ffi_libraries(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_ffi_attach_function_func;
extern int badger_ffi_attach_function(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_ffi_exposed_functions_func;
extern int badger_ffi_exposed_functions(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_ffi_exposed_function_func;
extern int badger_ffi_exposed_function(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_ffi_invoke_func;
extern int badger_ffi_invoke(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_ffi_close_func;
extern int badger_ffi_close(int argc,const badger_data_t *args,badger_caller_t *caller);

#endif
