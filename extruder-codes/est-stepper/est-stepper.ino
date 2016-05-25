#include "ext_Stepper_3D.h"

ext_stepper_3d extruder;

/* the ISR function is the one that does the moving of the stepper motor, it outputs the step at the time required */
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  extruder.inside_ISR();
}

void setup() 
{
	extruder.permission = 1;
	//digitalWrite(5,1);
	//digitalWrite(3,0);
	extruder.stepper_move(578,1500);
}

void loop() 
{

}
