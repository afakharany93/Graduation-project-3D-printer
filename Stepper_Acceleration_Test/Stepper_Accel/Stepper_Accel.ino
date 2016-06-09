#include "Stepper_3d.h"

 stepper_3d x;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
  Serial.flush();
 
  x.permission=1;
  x.Step_SetTime(600);

}

void loop() {
  
  // put your main code here, to run repeatedly:
  char y=0;
  if (y==0){
  x.stepper_move(1000);
  y++;
  }
}
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  x.inside_ISR();
 Serial.print(x.stepper_status());
Serial.print("\n");
}
