#include <badger/badger.h>
#include <badger/server.h>

#include <stdio.h>
#include <string.h>
#include <getopt.h>

int main(int argc,char *argv[])
{
	struct option options[] = {
		{"version", no_argument, NULL, 'V'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};
	int option_index = 0;
	int i;

	while ((i = getopt_long(argc,argv,"Vh",options,&option_index)) != -1)
	{
		switch (i)
		{
			case 'V':
				printf("%s: %s\n",argv[0],badger_version());
				return 0;
			case 'h':
				printf("%s usage: %s [options]\n",argv[0],argv[0]);
				return 0;
		}
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
