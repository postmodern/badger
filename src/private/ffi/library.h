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

#ifndef _PRIVATE_FFI_LIBRARY_H_
#define _PRIVATE_FFI_LIBRARY_H_

#if defined(_WIN32) || defined(__WIN32__)
# include <windows.h>
#else
# include <dlfcn.h>
#endif

#if defined(_WIN32) || defined(__WIN32__)

static void * dl_open(const char* name,int flags);
static void dl_error(char* buf,int size);

#define dl_sym(handle, name) GetProcAddress(handle, name)
#define dl_close(handle) FreeLibrary(handle)

enum {
	RTLD_LAZY = 1,
	RTLD_NOW,
	RTLD_GLOBAL,
	RTLD_LOCAL
};

#else

#define dl_open(name, flags) dlopen(name, flags != 0 ? flags : RTLD_LAZY)
#define dl_error(buf, size) do { snprintf(buf, size, "%s", dlerror()); } while(0)
#define dl_sym(handle, name) dlsym(handle, name)
#define dl_close(handle) dlclose(handle)

#ifndef RTLD_LOCAL
#define RTLD_LOCAL 8
#endif

#endif

#include <badger/slist.h>
#include "private/ffi/function.h"

struct ffi_library
{
	char *name;
	void *handler;

	slist_t *functions;
};
typedef struct ffi_library ffi_library_t;

ffi_library_t * ffi_library_open(const char *name);
int ffi_library_attach_function(ffi_library_t *lib,const char *name,const ffi_type *ret,int argc,const ffi_type **arg_types);
const ffi_function_t * ffi_library_search(const ffi_library_t *lib,const char *name);
void ffi_library_close(ffi_library_t *lib);

#endif
