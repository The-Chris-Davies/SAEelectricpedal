/*
map linear pot to rotary pot during calibration, then run checks to see if map fails

*/
#include "pedal.h"

void setup() {
	// initialize digital pin PB11 as an output.
	pinMode(PB11, OUTPUT);
	pinMode(PA7, INPUT);
	Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
	Serial.println("test start");
	digitalWrite(PB11, HIGH);
	Pedal ped(PB1, PB0);
	byte val;
	ped.calibrate(PA7);
	Serial.println("initialized");
	digitalWrite(PB11, HIGH);
	delay(1000);
	digitalWrite(PB11, HIGH);
	delay(1000);
	while(true){
		val = ped.read();
		Serial.println(val);
		analogWrite(PB11, val);
	}
}

