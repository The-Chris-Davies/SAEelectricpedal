/*
map linear pot to rotary pot during calibration, then run checks to see if map fails

*/
#include "pedal.h"
#include <libmaple/iwdg.h>

#define ROTARYPIN PA2
#define LINEARPIN PA1
#define APPSOUT PA0

void setup() {
	pinMode(PB13, INPUT);	//PB13: Calibration mode
	pinMode(PB14, INPUT);	//PB14: Run mode
	pinMode(APPSOUT, OUTPUT);	//set output pin
	Serial.begin(9600);
	delay(2000);
}

short val;

void loop() {
	Pedal ped(ROTARYPIN, LINEARPIN);

	//begin and initialize pedal controller
	if(digitalRead(PB13) && calibFlag){
		Serial.println("calibrating");
		ped.calibrate(PB13);
		Serial.println("done calib");
	}

	iwdg_init(IWDG_PRE_32, 10);	//init watchdog with value of ~8ms

	while(1):
		//reset watchdog
		iwdg_feed();

		//continuously read and print pedal value
		val = ped.read();
		if(val == -1){
			Serial.print ("Pedal Read Error!");
			analogWrite(APPSOUT, 0)
		}
		else {
			Serial.println(val);
			analogWrite(APPSOUT, val);
		}
	}
}
