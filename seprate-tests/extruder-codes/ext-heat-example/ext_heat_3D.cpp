/*
 * This is part of a graduation project solution aims to build a 3d printer,
 * made by students at the AASTMT Sheraton, Mechatronics department, 2016
 * https://github.com/afakharany93/Graduation-project-3D-printer
 *
 * Authors:
 * Ahmed El Fakharany - afakharany93@gmail.com
 *
 */



#include "ext_heat_3D.h"

Thermistor_3d ext_temp(A4);
fuzzy ext_fuzzy(9, 300, 0, 255, 0);


ext_heat::ext_heat()
{
	_permission = STOP_EXT_H;
	op_pin = 8;
	_old_time = 0;
	_current_time = millis();
	samp_time = 50;	//milliseconds
	input = 0;
	op = 0;
	ext_fuzzy.max_st = 500;
	ext_fuzzy.zero_st = 10;
	#if DEBUG_EXT_H
		
	#endif
}

void ext_heat::ext_heat_permission()
{
	_permission = START_EXT_H;
}

void ext_heat::ext_heat_control(unsigned int set_ext_temp)
{
	unsigned long diff;

	if (set_ext_temp < 2)
	{
		digitalWrite(op_pin, LOW);
		_permission = STOP_EXT_H;
		t = ext_temp.temperature_measurment();
		input = (int) t;
	}
	else if (_permission == START_EXT_H)
	{
		_current_time = millis();
		diff = _current_time - _old_time;
		samp_time = ext_fuzzy.sample_t_det ();
		if(diff >= samp_time)
		{
				t = ext_temp.temperature_measurment();
				input = (int) t;
				op = op + ext_fuzzy.fuzzy_controller(input, set_ext_temp);
				if(op > 255)
				{
					op = 255;
				}
				else if (op < 0)
				{
					op = 0;
				}
				analogWrite(op_pin, op);
			#if DEBUG_EXT_H
			  Serial.print(millis());
			  Serial.print("\t");
			  Serial.print(input);
			  Serial.print("\t");
			  Serial.print(ext_fuzzy.error);
			  Serial.print("\t");
			  Serial.print(ext_fuzzy.error_p);
			  Serial.print("\t");
			  Serial.print(ext_fuzzy.ch_error);
			  Serial.print("\t");
			  Serial.print(ext_fuzzy.ch_op);
			  Serial.print("\t");
			  Serial.print(op);
			  Serial.print("\t");
			  Serial.print(set_ext_temp);
			  Serial.print("\t");
			  Serial.print(ext_temp.res_debug_val);
			  Serial.print("\t");
			  Serial.println(samp_time);
			#endif


			_old_time = _current_time;
		}
	}
}

char * ext_heat::ext_heat_status()
{
	int x = 0;	//to hold the return value of sprintf
	char buff[75];
	x = sprintf(buff, "Status %u, ext_temp(c) %d",_permission , input);
	return (char *) buff;
}
