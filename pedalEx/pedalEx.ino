/*
map linear pot to rotary pot during calibration, then run checks to see if map fails

*/
#include "pedal.h"

#define ROTARYPIN PB1
#define LINEARPIN PB0

void setup() {
	//PB10 will be used as an input for calibration ISR
	pinMode(PB10, INPUT);
	Serial.begin(9600);
}

void loop() {
	
	//begin and initialize pedal controller
	delay(500);
	Serial.println("test start");
	digitalWrite(PC13, HIGH);
	
	Pedal ped(ROTARYPIN, LINEARPIN);
	short val;
	
	//calibrate pedal
	ped.calibrate();
	Serial.println("initialized");
	
	//blink LED to show calibration has exited
	digitalWrite(PC13, LOW);
	delay(1000);
	digitalWrite(PC13, HIGH);
	delay(1000);
	
	//continuously read and print pedal value
	while(true){
		val = ped.read();
		if(val == -1){
			Serial.print ("Pedal Read Error!");
		}
		Serial.println(val);
	}
}
