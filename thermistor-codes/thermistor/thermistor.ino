#define SERIES_RESISTOR		10000		//the value of the resistor in series with the thermistor in ohms

#define THERMISTOR_PIN		A0			//PIN CONNECTED TO THE THERMISTOR

#define AVERGAE_SAMPLES		10

void setup() {
  Serial.begin(115200);

}

void loop() {
	int ADC_value;
	double thermistor_resistance_value;
	double average = 0.0;

	for (int i = 0; i < AVERGAE_SAMPLES; i++ )
	{
		ADC_value = analogRead(THERMISTOR_PIN);
		Serial.print("Analog reading :"); 
	  	Serial.println(ADC_value);
		
		thermistor_resistance_value = (SERIES_RESISTOR * (float)ADC_value) / (1023 - (float)ADC_value);
		average += thermistor_resistance_value;
		
	}
	average = average / AVERGAE_SAMPLES;
	Serial.print("Thermistor resistance :"); 
	Serial.println(average);
  	delay(100);

}
