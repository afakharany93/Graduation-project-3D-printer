#include <math.h>

#define SERIES_RESISTOR		97900.0		//the value of the resistor in series with the thermistor in ohms

#define AVERGAE_SAMPLES		20			//used to hold the number of samples that are averaged

#define BETA_PARAMETER		3950.0	

#define	NOMINLA_THERMISTOR_VALUE	100000.0
#define NOMINAL_TEMPERATURE			298.15		//kelvin, 25 Celsius

#define SERIAL_DEBUG	1

void setup() {
#if SERIAL_DEBUG
  Serial.begin(115200);
#endif
}

void loop() {
	float temp = temperature_measurment();
	#if SERIAL_DEBUG
	Serial.print("Thermistor temp :"); 
	Serial.println(temp);
	#endif
  	delay(100);

}


double thermistor_measurment()
{
	int ADC_value;		//used to hold the value reading from the ADC
	double thermistor_resistance_value;		//used to hold the value of the resistance of the thermistor 
	long int average = 0;		//used to hold the averaged value
	int pin = A0;

	for (int i = 0; i < AVERGAE_SAMPLES; i++ )
	{
		ADC_value = analogRead(pin);
		//Serial.print("Analog reading :"); 
	  	//Serial.println(ADC_value);
	
		average += ADC_value;	
	}
	average = average / AVERGAE_SAMPLES;
	thermistor_resistance_value = (SERIES_RESISTOR * (float)average) / (1023.0 - (float)average);
	return thermistor_resistance_value;
}

float temperature_measurment()
{
	float temperature;
	temperature = thermistor_measurment();
	temperature = temperature / NOMINLA_THERMISTOR_VALUE;
	temperature = log(temperature);
	temperature = (1.0/BETA_PARAMETER) * temperature;
	temperature = (1.0/NOMINAL_TEMPERATURE) + temperature;
	temperature = (1.0 / temperature) - 273.15;	//Celsius
	return temperature;
}