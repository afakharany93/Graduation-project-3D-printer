#include "Stepper_3D.h"

 stepper_3d x;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
  Serial.flush();
 
  x.permission=1;
  x.Step_SetTime(550);
  x.stepper_move(10000);


}

void loop() {
  
  // put your main code here, to run repeatedly
  Serial.println(x.stepper_status());
  }

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  x.inside_ISR();
}
