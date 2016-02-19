/*if it works it was done by Ahmed El Fakharany to test the finite state machine concept with stepper motors for the 3d printer graduation project, 
if it doesn't workm I don't know who the hell did it but I'm sure it is not Ahmed El Fakharany :D */

#include "Stepper_3D.h"
#include "Global_variables.h"



stepper_3d extruder;
/*********** main and isr functions ***********/

void setup() 
{
	
}

/* the ISR function is the one that does the moving of the stepper motor, it outputs the step at the time required */
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
	if(stepper_steps > 0)	//if there are any remaining number of steps
	{
		if (direction == NEXT)
		{			
			extruder.stepper_output (&current_state);	//ouput the current state
			extruder.next_step(&current_state);			//point to the next state so that it can be outputed the next call of the isr
		}
		else if (direction == PREVIOUS)
		{
			extruder.stepper_output (&current_state);	//ouput the current state
			extruder.previos_step(&current_state);		//point to the previos state so that it can be outputed the next call of the isr
		}
		stepper_steps--;	//decrease the number of steps reaimed by one as it was just taken
	}
	else
	{
		stepper_steps = 0;	//just for safety
		TIMSK1 = 0;	//disable timer compare interrupt
		TCCR1B &= (~(1 << WGM12));   // disable timer CTC mode
	}
}

void loop() 
{
	extruder.stepper_flow (FORWARD);
	if (digitalRead(7) == HIGH)
	{
		extruder.stepper_stop();
	}
	if (digitalRead(6) == HIGH)
	{
		extruder.stepper_resume ();
	}
	
}



/************** Functions **************/

