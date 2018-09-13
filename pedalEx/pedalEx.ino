/*
map linear pot to rotary pot during calibration, then run checks to see if map fails

*/
#include "pedal.h"

#define ROTARYPIN PB1
#define LINEARPIN PB0

void setup() {
	// initialize digital pin ROTARYPIN1 as an output.
	pinMode(PB10, INPUT);
	Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
	delay(500);
	Serial.println("test start");
	digitalWrite(PC13, HIGH);
	Pedal ped(ROTARYPIN, LINEARPIN);
	byte val;
	ped.calibrate();
	Serial.println("initialized");
	digitalWrite(PC13, LOW);
	delay(1000);
	digitalWrite(PC13, HIGH);
	delay(1000);
	while(true){
		val = ped.read();
		Serial.println(val);
	}
}
