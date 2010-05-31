#include "private/ffi/library.h"
#include "private/util.h"

#include <malloc.h>

ffi_library_t * ffi_library_open(const char *name)
{
	char *new_name;

	if (!(new_name = strdup(name)))
	{
		// malloc failed
		goto cleanup;
	}

	ffi_library_t *new_lib;

	if (!(new_lib = malloc(sizeof(ffi_library_t))))
	{
		// malloc failed
		goto cleanup_name;
	}

	new_lib->name = new_name;

	if (!(new_lib->handler = dl_open(name,RTLD_LAZY)))
	{
		// dl_open failed
		badger_debug("ffi_library_open: could not open the given library (%s)\n",name);
		goto cleanup_lib;
	}

	new_lib->functions = slist_create(slist_compare_strings,(slist_destroy_func)ffi_function_destroy);
	return new_lib;

cleanup_lib:
	free(new_lib);
cleanup_name:
	free(new_name);
cleanup:
	return NULL;
}

int ffi_library_attach_function(ffi_library_t *lib,const char *name,const ffi_type *ret,int argc,const ffi_type **arg_types)
{
	void *ptr;

	if (!(ptr = dl_sym(lib->handler,name)))
	{
		// symbol not found
		badger_debug("ffi_library_attach_function: could not the given symbol (%s)\n",name);
		return -1;
	}

	ffi_function_t *new_function;

	if (!(new_function = ffi_function_create(name,ptr,ret,argc,arg_types)))
	{
		// malloc failed
		return -1;
	}

	return slist_add(lib->functions,new_function->name,new_function);
}

const ffi_function_t * ffi_library_search(const ffi_library_t *lib,const char *name)
{
	return (const ffi_function_t *)slist_search(lib->functions,name);
}

void ffi_library_close(ffi_library_t *lib)
{
	slist_destroy(lib->functions);

	if (lib->handler)
	{
		dl_close(lib->handler);
	}

	free(lib->name);

	// zero the library fields before freeing it
	lib->name = NULL;
	lib->handler = NULL;
	lib->functions = NULL;

	free(lib);
}
