/*
map linear pot to rotary pot during calibration, then run checks to see if map fails

*/
#include "pedal.h"

#define ROTARYPIN PA2
#define LINEARPIN PA1

void setup() {
	pinMode(PB13, INPUT);	//PB13: Calibration mode
	pinMode(PB14, INPUT);	//PB14: Run mode
	pinMode(PA0, OUTPUT);
	Serial.begin(9600);
}

Pedal ped(ROTARYPIN, LINEARPIN);
short val;

void loop() {
	
	//begin and initialize pedal controller
	if(digitalRead(PB13))
		ped.calibrate();
	
	//continuously read and print pedal value
	if(digitalRead(PB14)){
		val = ped.read();
		if(val == -1){
			Serial.print ("Pedal Read Error!");
		}
		Serial.println(val);
		analogWrite(PA0, val);
	}
}
