#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/neutrino.h>

#include "mystructs.h"

int main(int argc, char *argv[]) {
	int rcvid;
	int chid;
	int msg;

	chid = ChannelCreate(0);
	if(chid == -1){
		perror("Failed to create a channel.");
		exit(EXIT_FAILURE);
	}
	printf("Display startup:PID = %d\n", getpid());

	while(1){
		rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);
		MsgReply(rcvid, EOK, NULL, 0);

		printf("%s\n", outMessage[msg]);

		if(msg == STOP_MSG){
			exit(EXIT_SUCCESS);
		}
	}
	return EXIT_SUCCESS;
}
