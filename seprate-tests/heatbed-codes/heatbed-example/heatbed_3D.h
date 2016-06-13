/*
 * This is part of a graduation project solution aims to build a 3d printer,
 * made by students at the AASTMT Sheraton, Mechatronics department, 2016
 * https://github.com/afakharany93/Graduation-project-3D-printer
 *
 * Authors:
 * Ahmed El Fakharany - afakharany93@gmail.com
 *
 */



#ifndef __HEAT_BEEED__
#define __HEAT_BEEED__

#include "Arduino.h"
#include "fuzzy.h"
#include "Thermistor_3D.h"

#define START_HB	10
#define STOP_HB		20

#define DEBUG	1 	

class heatbed
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

	heatbed();
	void heatbed_permission();
	void heatbed_control(unsigned char set_temp);
	char * heatbed_status();

};

#endif
