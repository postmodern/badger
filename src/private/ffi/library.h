#ifndef _PRIVATE_FFI_LIBRARY_H_
#define _PRIVATE_FFI_LIBRARY_H_

#if defined(_WIN32) || defined(__WIN32__)
# include <windows.h>
#else
# include <dlfcn.h>
#endif

#include <badger/service.h>

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

struct ffi_library
{
	char *name;
	void *handler;

	slist_t *functions;
};
typedef struct ffi_library ffi_library_t;

ffi_library_t * ffi_library_open(const char *name);
void ffi_library_close(ffi_library_t *lib);

#endif
