#include "ext_heat_3D.h"
#include "ext_Stepper_3D.h"


ext_heat eh;
ext_stepper_3d extruder;


int st_point = 150;


/* the ISR function is the one that does the moving of the stepper motor, it outputs the step at the time required */
ISR(TIMER5_COMPA_vect)          // timer compare interrupt service routine
{
  extruder.inside_ISR();
}


void setup() 
{
	extruder.permission = 1;
	eh.ext_heat_permission();
	Serial.begin(115200);
  	Serial.flush();
 	Serial.println("time\ttemp\te\tep\tce\tcop\top\tsetP\tres\tst");
}

void loop() 
{
	eh.ext_heat_control(st_point);

	if(eh.input >= st_point)
	{
		extruder.stepper_move (-100, 164933 );
	}


}
