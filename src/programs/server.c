#include <badger/badger.h>
#include <badger/server.h>

#include <stdio.h>
#include <string.h>

int main(int argc,const char *args[])
{
	if (!strcmp(argv[1],"-V") || !strcmp(argv[1],"--version"))
	{
		printf("badger: %s\n",badger_version());
		return 0;
	}

	badger_init();

	badger_server_t *server;

	if (!(server = badger_server_create()))
	{
		fprintf(stderr,"malloc failed");
		return -1;
	}

	if (badger_server_open(server,argv[1]) == -1)
	{
	}

	badger_server_listen(server);

	badger_server_close(server);
	badger_server_destroy(server);

	badger_fini();
	return 0;
}
