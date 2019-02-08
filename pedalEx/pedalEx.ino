/*
map linear pot to rotary pot during calibration, then run checks to see if map fails

Pins:
PB13: calibration pin
PB14: run pin
PA04: CS
PA05: CLK
PA06: MISO
PA07: MOSI

*/

#include "Arduino.h"
#include <SPI.h>
#include "SdFat.h"
#include "pedal.h"
#include <libmaple/iwdg.h>

#define ROTARYPIN PA2
#define LINEARPIN PA1
#define APPSOUT PA0

/*
void blink();
short val;
bool sdStat;
Pedal ped;
*/
void setup() {
	
	void blink();
	short val;
	bool sdStat;
	Pedal ped;
	
	
	pinMode(PB13, INPUT);	//PB13: Calibration mode
	pinMode(PB14, INPUT);	//PB14: Run mode
	pinMode(PC13, OUTPUT);
	pinMode(APPSOUT, OUTPUT);	//set output pin
	Serial.begin(9600);
	while(!Serial){		//flash while waiting for serial
		digitalWrite(PC13, 1);
		delay(1000);
		digitalWrite(PC13, 0);
		delay(1000);
	}
	
	Serial.println("started!");
	
	SdFat card;
	SdFile datFile;
	
	sdStat = true;
	if (!card.begin(PA4, SD_SCK_MHZ(18))){
		Serial.println("error accessing SD card!");
		card.chvol();
		sdStat = false;
	}
	
	ped.setPins(ROTARYPIN, LINEARPIN);
	
	Serial.println("pedal pins set");
	
	if(!card.exists("/pedal.dat") && !digitalRead(PB13)) blink();	//if the file doesn't exist and calibration isn't happening, blink forever
	
	//begin and initialize pedal controller
	if(digitalRead(PB13)){
		Serial.println("calibrating");
		if(sdStat){
			if(!datFile.open("/pedal.dat", O_WRITE | O_CREAT)) blink();	//if the file can't be created for some reason, blink forever
			if(!ped.calibrate(PB13, datFile)){
				Serial.println("could not write to file!");
				blink();
			}
			datFile.close();
		}
		else {
			Serial.println("could not open file for writing");
			ped.calibrate(PB13);
		}
		Serial.println("done calib");
	}
	else{
		if(!datFile.open("/pedal.dat", O_READ)) blink();		//if the file can't be opened somehow, blink forever
		if(!ped.load(datFile)){
			Serial.println("could not load from file!");
			blink();
		}
		datFile.close();
	}
	
	
	//iwdg_init(IWDG_PRE_32, 10);	//init watchdog with value of ~8ms
	
	while(1){
		//reset watchdog
		//iwdg_feed();

		//continuously read and print pedal value
		val = ped.read();
		if(val == -1){
			Serial.print ("Pedal Read Error!");
			analogWrite(APPSOUT, 0);
		}
		else {
			Serial.println(val);
			analogWrite(APPSOUT, val);
		}
	
	}
	
}

void loop() {
	/*
		//reset watchdog
		iwdg_feed();

		//continuously read and print pedal value
		val = ped.read();
		if(val == -1){
			Serial.print ("Pedal Read Error!");
			analogWrite(APPSOUT, 0);
		}
		else {
			Serial.println(val);
			analogWrite(APPSOUT, val);
		}
	*/
}

void blink(){
	Serial.println("blinking forever");
	while(true){
		digitalWrite(PC13, 1);
		delay(500);
		digitalWrite(PC13, 0);
		delay(100);
	}
}
