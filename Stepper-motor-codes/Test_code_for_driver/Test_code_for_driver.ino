#define DELAY_T 1500
#define BLACK 9
#define BLUE 11
#define RED 10
#define GREEN 6
#define first GREEN
#define second   RED
#define third   BLACK 
#define forth  BLUE
#define PWMVal 255
/*// global variables
int first =  GREEN;
int second =  RED; 
int third =  BLACK ;
int forth = BLUE;*/


void setup() {

pinMode(first, OUTPUT);

pinMode(second, OUTPUT);

pinMode(third, OUTPUT);

pinMode(forth, OUTPUT);

pinMode(13,OUTPUT);

digitalWrite(first, LOW);

digitalWrite(third, LOW);

digitalWrite(second, LOW);

digitalWrite(forth, LOW);
digitalWrite(13, LOW);
}

void loop() {

analogWrite(first, PWMVal);

digitalWrite(second, LOW);

digitalWrite(third, LOW);

digitalWrite(forth, LOW);

digitalWrite(13,HIGH);

delayMicroseconds(DELAY_T);


digitalWrite(first, LOW);

analogWrite(second, PWMVal);

digitalWrite(third, LOW);

digitalWrite(forth, LOW);

digitalWrite(13,LOW);

delayMicroseconds(DELAY_T);


digitalWrite(first, LOW);

digitalWrite(second, LOW);

analogWrite(third, PWMVal);

digitalWrite(forth, LOW);

digitalWrite(13,HIGH);

delayMicroseconds(DELAY_T);


digitalWrite(first, LOW);

digitalWrite(second, LOW);

digitalWrite(third, LOW);

analogWrite(forth, PWMVal);

digitalWrite(13,LOW);
delayMicroseconds(DELAY_T);

}
