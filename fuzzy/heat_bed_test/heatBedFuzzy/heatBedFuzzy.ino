#include "Thermistor_3D.h"
#include "fuzzy.h"

Thermistor_3d temp(A0);
fuzzy magic(9, 180, 0, 255, 0);

int st_point = 100;
int input = 0;
int op = 0;

float t;

void setup() {
  // put your setup code here, to run once:
	Serial.begin(115200);


}

void loop() {
  // put your main code here, to run repeatedly:
  t = temp.temperature_measurment();
  input = (int) t;
  op = op + magic.fuzzy_controller(input, st_point);
  /*if(op > 255)
  {
  	op = 255;
  }
  else if (op < 0)
  {
  	op = 0;
  }*/
  analogWrite(9, op);
  
  Serial.print("temp: ");
  Serial.println(input);

  Serial.print("error: ");
  Serial.println(magic.error);
  Serial.print("error max: " );
  Serial.println(magic.error_max);
  Serial.print( "error min:  ");
  Serial.println(magic.error_min);
  Serial.print( "error percentized:  ");
  Serial.println(magic.error_p);


  Serial.print("ch error: ");
  Serial.println(magic.ch_error);
  Serial.print("ch error max: ");
  Serial.println(magic.ch_error_max );
  Serial.print("ch error min:  ");
  Serial.println(magic.ch_error_min);
  Serial.print("ch error percentized:  ");
  Serial.println(magic.ch_error_p);

  Serial.print("pwm: ");
  Serial.println(op);
  

  Serial.println("***********************************");
  delay(1000);
}
