#include <avr/interrupt.h>


volatile int count= 0;	//to count the umber of interrupts
void setup() {
      Serial.begin(115200); 
     PCICR |= 0b00000010;    // turn on port c change pin interrupt 
     PCMSK1 |= 0b00000001;    // turn on pin PC0, which is PCINT8, physical pin A0

}

void loop() {
//Serial.println(1-digitalRead(pin));
}


ISR(PCINT1_vect)
{
	count ++;
	//if((count%2))
	{
		Serial.print("press ");
		Serial.println(millis()); 
		Serial.println(count); 
		//Serial.println(1-digitalRead(pin));
	}
}