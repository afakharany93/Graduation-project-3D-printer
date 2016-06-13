#include "Thermistor_3D.h"

#define SERIAL_DEBUG	1

Thermistor_3d therm(A0);

void setup() {
therm.average_samples = 200;
#if SERIAL_DEBUG
  Serial.begin(115200);
#endif
}

void loop() {
	float temp = therm.temperature_measurment();
	#if SERIAL_DEBUG
	Serial.print("Thermistor temp :"); 
	Serial.println(temp);
	#endif
  	delay(100);

}

