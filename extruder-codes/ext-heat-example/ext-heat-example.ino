#include "Thermistor_3D.h"
#include "fuzzy.h"
#include "ext_Stepper_3D.h"


Thermistor_3d temp(A0);
fuzzy magic(9, 300, 0, 255, 0);
ext_stepper_3d extruder;


int st_point = 100;
int input = 0;
int op = 0;

float t;
int st;

/* the ISR function is the one that does the moving of the stepper motor, it outputs the step at the time required */
ISR(TIMER5_COMPA_vect)          // timer compare interrupt service routine
{
  extruder.inside_ISR();
}


void setup() 
{
	extruder.permission = 1;
	Serial.begin(115200);
  	Serial.flush();
 	Serial.println("time\ttemp\te\tep\tce\tcop\top\tsetP\tres\tst");
}

void loop() 
{
	t = temp.temperature_measurment();
	input = (int) t;
	if(input != -273)	//when sensor breaks down, -273 is the reading, so this is a protection line
	{
		op = op + magic.fuzzy_controller(input, st_point);
		if(op > 255)
		{
			op = 255;
		}
		else if (op < 0)
		{
		 	op = 0;
		}
	}
	else
	{
		op = 0;
	}
	analogWrite(3, op);

	if(input >= st_point)
	{
		extruder.stepper_flow(extruder.anticlockwise);
	}


	st = magic.sample_t_det ();  

	Serial.print(millis());
	Serial.print("\t");
	Serial.print(input);
	Serial.print("\t");
	Serial.print(magic.error);
	Serial.print("\t");
	Serial.print(magic.error_p);
	Serial.print("\t");
	Serial.print(magic.ch_error);
	Serial.print("\t");
	Serial.print(magic.ch_op);
	Serial.print("\t");
	Serial.print(op);
	Serial.print("\t");
	Serial.print(st_point);
	Serial.print("\t");
	Serial.print(temp.res_debug_val);
	Serial.print("\t");
	Serial.println(st);

	delay(st);
}
