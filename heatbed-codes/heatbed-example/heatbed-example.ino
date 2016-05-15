#include "heatbed_3D.h"

unsigned long ot = 0;
unsigned long ct;
unsigned long d;

heatbed bed ;
void setup() {
  // put your setup code here, to run once:
  bed.heatbed_permission();
  Serial.begin(115200);
  Serial.println("time\ttemp\te\tep\tce\tcop\top\tsetP\tres\tst");
}

void loop() 
{
	bed.heatbed_control(100);
	
}
