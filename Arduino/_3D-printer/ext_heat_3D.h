/*
 * This is part of a graduation project solution aims to build a 3d printer,
 * made by students at the AASTMT Sheraton, Mechatronics department, 2016
 * https://github.com/afakharany93/Graduation-project-3D-printer
 *
 * Authors:
 * Ahmed El Fakharany - afakharany93@gmail.com
 *
 */



#ifndef __HEAT_EXTRUDER__
#define __HEAT_EXTRUDER__

#include "Arduino.h"
#include "fuzzy.h"
#include "Thermistor_3D.h"

#define START_EXT_H		30
#define STOP_EXT_H		40

#define DEBUG_EXT_H		0 	

class ext_heat
{
private:
	unsigned char _permission;
	unsigned long _old_time;
	unsigned long _current_time;

public:
	unsigned char op_pin;
	unsigned long samp_time;
	float t;
	int input;
	int op ;

	ext_heat();
	void ext_heat_permission();
	void ext_heat_control(unsigned int set_temp);
	char * ext_heat_status();

};

#endif
