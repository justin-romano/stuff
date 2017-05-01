
#include "SevSeg.h"
#define F_CPU 16000000

SevSeg sevseg; //Initiate a seven segment controller object
void setup() {

	byte numDigits = 4;

	byte digitPins[] = {2, 3, 4, 5};

	byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};

	sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins);

	sevseg.setBrightness(90);
}
float f = 0.0;
void loop() {
	
	sevseg.setNumber(f, 2);
	f += 0.1;
	sevseg.refreshDisplay(1000);
}