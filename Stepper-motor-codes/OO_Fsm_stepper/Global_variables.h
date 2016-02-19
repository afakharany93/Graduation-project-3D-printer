#ifndef _GLOBAL_VARIABLES_
#define _GLOBAL_VARIABLES_

#include "Arduino.h"

//for stpeer motor
extern struct stepper_state_struct current_state;		//the variable that will hold the current state information, initialized with state zero info
extern unsigned long int 	stepper_steps;			//this variable holds the number of steps remained to be moved, needed by the isr
extern unsigned char 		direction;				//this variable holds the direction of movement, needed by the isr
extern unsigned char		stepper_permission;		//used to prevent stepper_move function fromoverwriting itself, to execute stepper_move set it to 1, to stop the overwrting set it to 0

#endif