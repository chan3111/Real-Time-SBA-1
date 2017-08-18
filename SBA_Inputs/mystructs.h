/*
 * mystructs.h
 *
 *  Created on: Mar 9, 2017
 *      Author: Chandler Newman-Reed
 */

#ifndef MYSTRUCTS_H_

#define NUM_STATES 8
typedef enum {
	START_STATE = 0,
	READY_STATE = 1,
	LEFT_DOWN_STATE = 2,
	RIGHT_DOWN_STATE = 3,
	ARMED_STATE = 4,
	PUNCH_STATE = 5,
	EXIT_STATE = 6,
	STOP_STATE = 7
} State;

#define NUM_INPUTS 5
typedef enum {
	LEFT_BUTTON_DOWN = 0,
	LEFT_BUTTON_UP = 1,
	RIGHT_BUTTON_DOWN = 2,
	RIGHT_BUTTON_UP = 3,
	STOP_BUTTON = 4
} Input;

#define NUM_OUTPUTS 10
typedef enum {
	START_MSG = 0,
	READY_MSG = 1,
	LEFT_DOWN_MSG = 2,
	RIGHT_DOWN_MSG = 3,
	LEFT_UP_MSG = 4,
	RIGHT_UP_MSG = 5,
	ARMED_MSG = 6,
	PUNCH_MSG = 7,
	EXIT_MSG = 8,
	STOP_MSG = 9
} Output;

const char *outMessage[NUM_OUTPUTS] = {
		"Start",
		"Ready",
		"Left Button Down",
		"Right Button Down",
		"Left Button Up",
		"Right Button Up",
		"DANGER = Press Armed",
		"Punching.",
		"Exiting.",
		"Stop Message."
};

const char *inMessage[NUM_INPUTS] = {
		"LD",
		"LU",
		"RD",
		"RU",
		"S"
};

#define MYSTRUCTS_H_
#endif /* MYSTRUCTS_H_ */
