#include "heatbed_3D.h"

heatbed bed ;
void setup() {
  // put your setup code here, to run once:
  bed.heatbed_permission();
  Serial.begin(115200);
  Serial.println("time\ttemp\te\tep\tce\tcop\top\tsetP");
}

void loop() {
  // put your main code here, to run repeatedly:
	bed.heatbed_control(105);

}
