/*
 * This is part of a graduation project solution aims to build a 3d printer,
 * made by students at the AASTMT Sheraton, Mechatronics department, 2016
 * https://github.com/afakharany93/Graduation-project-3D-printer
 *
 * Authors:
 * Ahmed El Fakharany - afakharany93@gmail.com
 *
 */



#include "heatbed_3D.h"

Thermistor_3d temp(A0);
fuzzy magic(9, 120, 0, 220, 0);


heatbed::heatbed()
{
	_permission = STOP_HB;
	op_pin = 9;
	_old_time = 0;
	_current_time = millis();
	samp_time = 500;	//milliseconds
	input = 0;
	op = 0;
	#if DEBUG
		
	#endif
}

void heatbed::heatbed_permission()
{
	_permission = START_HB;
}

void heatbed::heatbed_control(unsigned char set_temp)
{
	unsigned long diff;

	if (set_temp < 2)
	{
		digitalWrite(op_pin, LOW);
		_permission = STOP_HB;
		t = temp.temperature_measurment();
		input = (int) t;
	}
	else if (_permission == START_HB)
	{
		_current_time = millis();
		diff = _current_time - _old_time;
		if(diff >= samp_time)
		{
				t = temp.temperature_measurment();
				input = (int) t;
				op = op + magic.fuzzy_controller(input, set_temp);
				if(op > 220)
				{
					op = 220;
				}
				else if (op < 0)
				{
					op = 0;
				}
				analogWrite(op_pin, op);
			#if DEBUG
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
			  Serial.println(set_temp);
			#endif


			_old_time = _current_time;
		}
	}
}

char * heatbed::heatbed_status()
{
	int x = 0;	//to hold the return value of sprintf
	char buff[75];
	x = sprintf(buff, "Status %u, temp(c) %d",_permission , input);
	return (char *) buff;
}
