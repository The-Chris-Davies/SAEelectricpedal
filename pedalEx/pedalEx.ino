/*
map linear pot to rotary pot during calibration, then run checks to see if map fails

*/
#include "pedal.h"

#define ROTARYPIN PA2
#define LINEARPIN PA1

void setup() {
	pinMode(PB13, INPUT);	//PB13: Calibration mode
	pinMode(PB14, INPUT);	//PB14: Run mode
	Serial.begin(9600);
	delay(2000);
}
short val;

void loop() {
	Pedal ped(ROTARYPIN, LINEARPIN);
	bool calibFlag = false;
	while(1){
		//begin and initialize pedal controller
		if(digitalRead(PB13) && !calibFlag){
			Serial.println("calibrating");
			ped.calibrate(PB13);
			calibFlag = true;
			Serial.println("done calib");
		}
		
		//continuously read and print pedal value
		if(digitalRead(PB14)){
			val = ped.read();
			if(val == -1){
				Serial.print ("Pedal Read Error!");
			}
			Serial.println(val);
		}
	}
}
