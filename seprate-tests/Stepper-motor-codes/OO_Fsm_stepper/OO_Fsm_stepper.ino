/*
this file is just to test the library
if it works it was done by Ahmed El Fakharany to test the finite state machine concept with stepper motors for the 3d printer graduation project, 
if it doesn't workm I don't know who the hell did it but I'm sure it is not Ahmed El Fakharany :D */

#include "Stepper_3D.h"

#define FORWARD		extruder.forward 
#define BACKWARD 	extruder.backward


stepper_3d extruder;
/*********** main and isr functions ***********/

void setup() 
{
	extruder.white = 13;
	extruder.green = 12;
	extruder.brown = 11;
	extruder.blue = 10;
	extruder.first = extruder.blue;
	extruder.second = extruder.brown;
	extruder.third = extruder.green;
	extruder.forth = extruder.white;
	extruder.permission = 1;
	extruder.stepper_move (-5*800,2525);
	Serial.begin(115200);
	
}

/* the ISR function is the one that does the moving of the stepper motor, it outputs the step at the time required */
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
	extruder.inside_ISR();

}

void loop() 
{
	/*if (digitalRead(7) == HIGH)
	{
		extruder.stepper_stop();
	}
	if (digitalRead(6) == HIGH)
	{
		extruder.stepper_resume ();
	}*/
	delay(50);
	Serial.println(extruder.stepper_status());
	//extruder.stepper_flow (BACKWARD);
}




