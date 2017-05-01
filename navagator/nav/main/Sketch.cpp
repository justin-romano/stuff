/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */


//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio
#define B_1A 11
#define B_1B 10

void setup()
{
	pinMode(B_1A,OUTPUT);
	pinMode(B_1B,OUTPUT);
	digitalWrite(B_1A, LOW);
	digitalWrite(B_1B,LOW);
	Serial.begin(9600);
	
}

void runFor(int power, int forMs, int direction)
{
	if(direction == 1)
		power -=1;
	int pin = (direction == -1 ? B_1A : B_1B);
	analogWrite(pin,power += direction);
	delay(forMs);
	analogWrite(pin,0);
	digitalWrite(pin, LOW);
}

void loop()
{
	runFor(234,150,1);
	delay(100);
	runFor(234,150,1);
	delay(100);
	runFor(234,150,1);
	delay(100);
	runFor(234,150,-1);
	delay(100);
	runFor(234,150,-1);
	delay(100);
	runFor(234,150,-1);
	delay(100);
}