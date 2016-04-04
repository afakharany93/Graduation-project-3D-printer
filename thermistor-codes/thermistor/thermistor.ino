#include <math.h>

#define SERIES_RESISTOR		10000		//the value of the resistor in series with the thermistor in ohms

#define THERMISTOR_PIN		A0			//PIN CONNECTED TO THE THERMISTOR

#define AVERGAE_SAMPLES		10			//used to hold the number of samples that are averaged

#define BETA_PARAMETER		3950	

#define	NOMINLA_THERMISTOR_VALUE	100000
#define NOMINAL_TEMPERATURE			298.15		//kelvin, 25 Celsius

void setup() {
  Serial.begin(115200);

}

void loop() {
	
	Serial.print("Thermistor temp :"); 
	Serial.println(temperature_measurment());
  	delay(100);

}


double thermistor_measurment()
{
	int ADC_value;		//used to hold the value reading from the ADC
	double thermistor_resistance_value;		//used to hold the value of the resistance of the thermistor 
	int average = 0.0;		//used to hold the averaged value
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

double temperature_measurment()
{
	double resistance;
	double temperature;
	resistance = thermistor_measurment();
	temperature = (1.0/NOMINAL_TEMPERATURE) + ((1.0/BETA_PARAMETER) * log(resistance / NOMINLA_THERMISTOR_VALUE));
	temperature = (1.0 / temperature) - 273.15;	//Celsius
	return temperature;
}

