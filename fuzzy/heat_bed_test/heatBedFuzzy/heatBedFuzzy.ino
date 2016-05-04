#include "Thermistor_3D.h"
#include "fuzzy.h"

Thermistor_3d temp(A0);
fuzzy magic(9, 120, 0, 220, 0);

int st_point = 100;
int input = 0;
int op = 0;

float t;

void setup() {
  // put your setup code here, to run once:
	Serial.begin(115200);
  Serial.flush();
  Serial.println("time\ttemp\te\tep\tce\tcop\top\tsetP");

}

void loop() {
  // put your main code here, to run repeatedly:
  t = temp.temperature_measurment();
  input = (int) t;
  //analogWrite(9, 220); //to get 15v pout of 24v ps use thyis pwm
  op = op + magic.fuzzy_controller(input, st_point);
  if(op > 220)
  {
  	op = 220;
  }
  else if (op < 0)
  {
  	op = 0;
  }
  analogWrite(9, op);
  

  Serial.print(millis());
  Serial.print("\t");
  Serial.print(input);
  Serial.print("\t");
  Serial.print(magic.error);
  Serial.print("\t");
  Serial.print(magic.error_p);
  Serial.print("\t");
  Serial.print(magic.ch_error);
  Serial.print("\t");
  Serial.print(magic.ch_op);
  Serial.print("\t");
  Serial.print(op);
  Serial.print("\t");
  Serial.println(st_point);
  /*  Serial.print("temp: ");
  Serial.println(input);

  Serial.println(" ");
  Serial.println(magic.error);
  Serial.print("error max: " );
  Serial.println(magic.error_max);
  Serial.print( "error min:  ");
  Serial.println(magic.error_min);
  Serial.print( "error percentized:  ");
  Serial.println(magic.error_p);

  Serial.println(" ");

  Serial.print("ch error: ");
  Serial.println(magic.ch_error);
  Serial.print("ch error max: ");
  Serial.println(magic.ch_error_max );
  Serial.print("ch error min:  ");
  Serial.println(magic.ch_error_min);
  Serial.print("ch error percentized:  ");
  Serial.println(magic.ch_error_p);

	Serial.println(" ");

  Serial.print("1st set: ");
  Serial.println(magic.err_set.set_1);
  Serial.print("1st set u: ");
  Serial.println(magic.err_set.deg_truth_1);
  Serial.print("2nd set: ");
  Serial.println( magic.err_set.set_2);
  Serial.print("2nd set u: ");
  Serial.println(magic.err_set.deg_truth_2);

Serial.println(" ");

  Serial.print("ch 1st set: ");
  Serial.println(magic.ch_err_set.set_1);
  Serial.print("ch 1st set u: ");
  Serial.println(magic.ch_err_set.deg_truth_1);
  Serial.print("ch 2nd set: ");
  Serial.println(magic.ch_err_set.set_2);
  Serial.print("ch 2nd set u: ");
  Serial.println(magic.ch_err_set.deg_truth_2);

Serial.println(" ");

  Serial.print("ch output 1st set: ");
  Serial.println(magic.ch_op_set.set_1);
  Serial.print("ch output 1st set u: ");
  Serial.println(magic.ch_op_set.deg_truth_1);
  Serial.print("ch output 2nd set: ");
  Serial.println(magic.ch_op_set.set_2);
  Serial.print("ch output 2nd set u: ");
  Serial.println(magic.ch_op_set.deg_truth_2);
  Serial.print("ch output 3rd set: ");
  Serial.println(magic.ch_op_set.set_3);
  Serial.print("ch output 3rd set u: ");
  Serial.println(magic.ch_op_set.deg_truth_3);

Serial.println(" ");

  Serial.print("ch output percentized: ");
  Serial.println(magic.ch_op_p);
  Serial.print("ch output: ");
  Serial.println(magic.ch_op);

  Serial.print("pwm: ");
  Serial.println(op);
  
   Serial.print("temp: ");
  Serial.println(input);
   Serial.print("time: ");
  Serial.println(millis());
  Serial.println("***********************************");*/
  delay(500);
}
