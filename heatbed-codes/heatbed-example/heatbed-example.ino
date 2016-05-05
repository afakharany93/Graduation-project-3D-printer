#include "heatbed_3D.h"

unsigned long ot = 0;
unsigned long ct;
unsigned long d;

heatbed bed ;
void setup() {
  // put your setup code here, to run once:
  bed.heatbed_permission();
  Serial.begin(115200);
  Serial.println("time\ttemp\te\tep\tce\tcop\top\tsetP");
}

void loop() {
	ct = millis();
	d = ct - ot;
	if(d <= 300000)
	{
		
		bed.heatbed_control(60);
	}
	else if (d <= 900000)
	{
		
		bed.heatbed_control(50);
	}
	else 
	{
		
		bed.heatbed_control(70);
	}

}
