#include "Stepper_3D.h"

 stepper_3d x;
  int y=0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
  Serial.flush();
 
  x.permission=1;


    x.Step_SetTime(500);
  x.stepper_move(1000);

}

void loop() {
 // int z=350;
 // x.Step_SetTime(1000);
//Serial.println(x.stepper_status());
 //delay(50);
  // put your main code here, to run repeatedly
 /*for(y;y<z;y=y+25){
 x.Step_SetTime(800-y);
    Serial.println(x.stepper_status());
  //Serial.print(x.current_time_bet_steps);
delay(100);
//y++;
  }
  if(y==z)
  {
  Serial.println(x.stepper_status());
  delay(50);
  }
  */
}
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  x.inside_ISR();
  Serial.println(x.stepper_status());
}
