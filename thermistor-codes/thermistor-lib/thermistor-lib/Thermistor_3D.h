/*
 * This is part of a graduation project solution aims to build a 3d printer,
 * made by students at the AASTMT Sheraton, Mechatronics department, 2016
 * https://github.com/afakharany93/Graduation-project-3D-printer
 *
 * Authors:
 * Ahmed El Fakharany - afakharany93@gmail.com
 ***********************************************************************
* thermistor connection
* (Ground) ---- (Thermistor) -------|------- (series Resistor) ---- (+5v)
*                                   |
*                               Analog Pin 
* 
* thermistor resistance = (series resistance * ADC value) / (1023 - ADC value) 
*/

#ifndef _THERMISTOR_3D_
#define _THERMISTOR_3D_

#include "Arduino.h"
#include <math.h>

class Thermistor_3d
{
	public:
		Thermistor_3d(unsigned char pin);
		float thermistor_measurment();
		float temperature_measurment();

		float series_resistor = 97900.0;		//used to hold the value of the series resistor in ohms
		unsigned char average_samples = 20;		//variable used to hold the number of samples taken to calculate the average
		float beta = 3950.0;					//used to hold the value of the beta parameter -from thermistor datasheet- to calculate temperature 
		float nom_thermistor_resistance = 100000.0;	//used to hold the nominal value of the thermistor resistance @ 25 c
		float nom_temperature = 298.15;		//kelvin, 25 Celsius
	private:
		unsigned char _pin;		//used to hold the analog pin used for reading
};

#endif 