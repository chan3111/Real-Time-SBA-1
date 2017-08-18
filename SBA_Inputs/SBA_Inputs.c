#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>

#include "mystructs.h"

int main(int argc, char *argv[]) {
	int coid;
	pid_t serverpid = atoi(argv[1]);
	char msg[5];

	coid = ConnectAttach(ND_LOCAL_NODE, serverpid, 1, _NTO_SIDE_CHANNEL, 0);
	if (coid == -1) {
		fprintf (stderr, "Couldn't ConnectAttach\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}
	while(1){
		printf("Enter the event type (LD - Left Button Down, RD - Right Button Down, "
				"LU - Left Button Up, RU - Right Button Up, S - Stop Button)\n");
		scanf("%s", msg);

		if(MsgSend(coid, &msg, sizeof(msg), NULL, 0) == -1){
			fprintf (stderr, "Error during MsgSend\n");
			perror (NULL);
			exit (EXIT_FAILURE);
		}

		if(strcmp(msg, "S") == 0){
			exit(EXIT_SUCCESS);
		}
	}

	return EXIT_SUCCESS;
}
