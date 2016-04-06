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


#include "Thermistor_3D.h"

Thermistor_3d::Thermistor_3d(unsigned char pin)
{
	_pin = pin;
}

float Thermistor_3d::thermistor_measurment()
{
	int ADC_value;		//used to hold the value reading from the ADC
	float thermistor_resistance_value;		//used to hold the value of the resistance of the thermistor 
	long int average = 0;		//used to hold the averaged value
	int pin = A0;

	for (int i = 0; i < average_samples; i++ )	//sampling ADC readings
	{
		ADC_value = analogRead(pin);
		average += ADC_value;	
	}
	average = average / average_samples;	//averaging ADC readings
	thermistor_resistance_value = (series_resistor * (float)average) / (1023.0 - (float)average);	//calculating resistance value of the thermistor
	return thermistor_resistance_value;
}

float Thermistor_3d::temperature_measurment()	//using beta parameter method
{
	float temperature;
	temperature = thermistor_measurment();
	temperature = temperature / nom_thermistor_resistance;
	temperature = log(temperature);
	temperature = (1.0/beta) * temperature;
	temperature = (1.0/nom_temperature) + temperature;
	temperature = (1.0 / temperature) - 273.15;	//Celsius
	return temperature;
}
