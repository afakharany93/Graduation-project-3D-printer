int pin = 2;	//dio pin which accepts external interrupts are either 2 or 3

volatile int count= 0;	//to count the umber of interrupts
void setup() {
    pinMode(pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(pin), report, FALLING);
      Serial.begin(115200);  
}

void loop() {
//Serial.println(1-digitalRead(pin));
}

void report() {
uint8_t oldSREG = SREG;
cli();
//detachInterrupt(digitalPinToInterrupt(pin));
//Serial.println(1-digitalRead(pin));
delay(1);
//Serial.println(1-digitalRead(pin));

if(!digitalRead(pin))
{
	count ++;
	//if((count%2))
	{
		//Serial.print("press ");
		//Serial.println(millis()); 
		//Serial.println(count); 
		Serial.println(1-digitalRead(pin));
	}
}
 SREG = oldSREG;
}