#include <badger/badger.h>
#include <badger/server.h>
#include <badger/services.h>

#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	struct option options[] = {
		{"connect", required_argument, NULL, 'c'},
		{"listen", required_argument, NULL, 'l'},
		{"version", no_argument, NULL, 'V'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};
	int option_index = 0;
	int i;

	badger_mode server_mode = badger_mode_none;
	const char *server_uri = NULL;

	while ((i = getopt_long(argc,argv,"l:c:Vh",options,&option_index)) != -1)
	{
		switch (i)
		{
			case 'c':
				server_mode = badger_mode_connect;
				server_uri = optarg;
				break;
			case 'l':
				server_mode = badger_mode_listen;
				server_uri = optarg;
				break;
			case 'V':
				printf("%s: %s\n",argv[0],badger_version());
				return 0;
			case 'h':
				printf("%s usage: %s [options]\n",argv[0],argv[0]);
				return 0;
		}
	}

	if (server_mode == badger_mode_none || !server_uri)
	{
		fprintf(stderr,"%s: must specify either the -c or -l option\n",argv[0]);
		goto cleanup;
	}

	badger_init();

	badger_server_t *server;

	if (!(server = badger_server_create()))
	{
		fprintf(stderr,"malloc failed");
		goto cleanup;
	}

#ifdef SERVICES_SYS
	if (badger_server_register(server,&badger_sys_service) == -1)
	{
		goto cleanup_server;
	}
#endif

#ifdef SERVICES_FS
	if (badger_server_register(server,&badger_fs_service) == -1)
	{
		goto cleanup_server;
	}
#endif

#ifdef SERVICES_FFI
	if (badger_server_register(server,&badger_ffi_service) == -1)
	{
		goto cleanup_server;
	}
#endif

	if (badger_server_open(server,server_mode,server_uri) == -1)
	{
		goto cleanup_server;
	}

	badger_server_loop(server);
	
	badger_server_close(server);

	badger_server_destroy(server);

	badger_fini();
	return 0;

cleanup_server:
	badger_server_destroy(server);
cleanup:
	return -1;
}
