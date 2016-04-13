#include <avr/interrupt.h>


volatile int count= 0;	//to count the umber of interrupts
void setup() {
      Serial.begin(115200);
      /* turn on pin PCINT9 and PCINT10 p[in change interrupts, which is: 
     		for NANO : PC1 and PC2, physical pin A1 and A2
     		for MEGA : Pj0 and Pj1, physical pins 15 and 14*/ 
     PCICR |= 0b00000010;    
     PCMSK1 |= 0b00000110;    

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