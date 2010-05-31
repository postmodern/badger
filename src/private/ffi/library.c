#include "private/ffi/library.h"

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

	if (!(new_lib->handler = dl_open(name,0)))
	{
		// dl_open failed
		goto cleanup_lib;
	}

	new_lib->functions = slist_create(slist_compare_strings,NULL);
	return new_lib;

cleanup_lib:
	free(new_lib);
cleanup_name:
	free(new_name);
cleanup:
	return NULL;
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
