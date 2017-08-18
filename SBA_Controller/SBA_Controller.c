#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>

#include "mystructs.h"

int coid;
int msg_send;
int rcvid;

// States
void *start_state(char*);
void *ready_state(char*);
void *left_down_state(char*);
void *right_down_state(char*);
void *armed_state(char*);
void *punch_state(char*);
void *exit_state(char*);
void *stop_state(char*);

// Functions
void sendDisplay(int);

typedef void *(*StateFunc)(char *);

int main(int argc, char *argv[]) {
	int chid;
	pid_t displaypid = atoi(argv[1]);
	char msg[5];

	StateFunc states;

	chid = ChannelCreate(0);
	if(chid == -1){
		perror("Failed to create a channel.\n");
		exit(EXIT_FAILURE);
	}

	coid = ConnectAttach(ND_LOCAL_NODE, displaypid, 1, _NTO_SIDE_CHANNEL, 0);
	if(coid == -1){
		fprintf(stderr, "Couldn't attach to client.\n");
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	printf("Controller startup:PID = %d\n", getpid());

	states = start_state(msg);
	while(1){
		rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);
		states = (StateFunc)(*states)(msg);
		MsgReply(rcvid, EOK, NULL, 0);
	}

	return EXIT_SUCCESS;
}

// Start state
void *start_state(char* msg){
	sendDisplay(START_MSG);
	return ready_state(msg);
}

// Ready state
void *ready_state(char* msg){
	sendDisplay(READY_MSG);
	sleep(3);
	if(strcmp(msg, inMessage[STOP_BUTTON]) == 0){
		sendDisplay(EXIT_MSG);
		exit_state(msg);
	} else if(strcmp(msg, inMessage[LEFT_BUTTON_DOWN]) == 0){
		sendDisplay(LEFT_DOWN_MSG);
		return left_down_state;
	} else if(strcmp(msg, inMessage[RIGHT_BUTTON_DOWN]) == 0){
		sendDisplay(RIGHT_DOWN_MSG);
		return right_down_state;
	}
	return ready_state;
}

// Left button down state
void *left_down_state(char* msg){
	if(strcmp(msg, inMessage[LEFT_BUTTON_UP]) == 0){
		sendDisplay(LEFT_UP_MSG);
		return ready_state;
	} else if(strcmp(msg, inMessage[RIGHT_BUTTON_DOWN]) == 0){
		sendDisplay(RIGHT_DOWN_MSG);
		return armed_state(msg);
	}
	return left_down_state;
}

// Right button down state
void *right_down_state(char *msg){
	if(strcmp(msg, inMessage[RIGHT_BUTTON_UP]) == 0){
		sendDisplay(RIGHT_UP_MSG);
		return ready_state;
	} else if (strcmp(msg, inMessage[LEFT_BUTTON_DOWN]) == 0){
		sendDisplay(LEFT_DOWN_MSG);
		return armed_state(msg);
	}
	return right_down_state;
}

// Armed state
void *armed_state(char *msg){
	sendDisplay(ARMED_MSG);
	sleep(2);
	if(strcmp(msg, inMessage[LEFT_BUTTON_UP])==0){
		sendDisplay(READY_MSG);
		return ready_state;
	} else if(strcmp(msg, inMessage[RIGHT_BUTTON_UP])==0){
		sendDisplay(READY_MSG);
		return ready_state;
	} else {
		return punch_state(msg);
	}
}

// Punch state
void *punch_state(char* msg){
	sendDisplay(PUNCH_MSG);
	sleep(1);
	return ready_state;
}

// Exit state
void *exit_state(char* msg){
	sendDisplay(STOP_MSG);
	stop_state(msg);
	return stop_state;
}

// Stop state
void *stop_state(char* msg){
	MsgReply(rcvid, EOK, NULL, 0);
	exit(EXIT_SUCCESS);
}

// Function for easily sending message to display program
void sendDisplay(int smsg){
	msg_send = smsg;
	if(MsgSend(coid, &msg_send, sizeof(msg_send), NULL, 0) == -1){
		fprintf (stderr, "Error during MsgSend\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}
}
