#include "ext_Stepper_3D.h"

ext_stepper_3d extruder;

/* the ISR function is the one that does the moving of the stepper motor, it outputs the step at the time required */
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  extruder.inside_ISR();
}

void setup() 
{
	pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(6,1);
  digitalWrite(7,1);
	extruder.permission = 1;
	//digitalWrite(5,1);
	//digitalWrite(3,0);
	extruder.stepper_move(5000,16000);
}

void loop() 
{

}
