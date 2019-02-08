/*
	Pedal.cpp : a library to control the pedal for an electric car
	maps linear pot to rotary pot during calibration, then run checks to see if map fails
	
	rotary pot: clarostat hrs100swab090
	linear pot: ???
	
	Chris Davies, sept 2018
*/

#include "Arduino.h"
#include <SPI.h>
#include "SdFat.h"
#include "pedal.h"

volatile bool errFlag;	//if the pedal has a read error > 100ms

void setErr(){
	errFlag = true;
}

Pedal::Pedal(){
	delay(1000);
	//value initialization
	mini = -1;
	maxi = 0;
	err = 0;
	dZone[0] = 20;
	dZone[1] = 20;
	for(unsigned int i = 0; i < 256; i++) potVal[i] = 0;
		
	//100ms timer for error
	Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
	Timer2.setPeriod(100000);	// 100Hz in microseconds
	Timer2.attachCompare1Interrupt(setErr);
	Timer2.pause();
}

Pedal::Pedal(int rotaryPin, int linearPin){
	Pedal();
	setPins(rotaryPin, linearPin);
}

void Pedal::setPins(int rotaryPin, int linearPin){
	rot = rotaryPin;
	lin = linearPin;

}

//returns true if error is within acceptable levels
inline bool Pedal::check(uint8_t rotVal, uint8_t linVal){
	return (potVal[rotVal] < linVal+err && potVal[rotVal] > linVal-err);
}

void Pedal::calibrate(int interPin){
	digitalWrite(PC13, 1);		//turn on LED while initializing
	uint8_t currVal, linVal;
	
	//reset errFlag and timer
	errFlag = false;
	Timer2.pause();
	Timer2.refresh();
	
	//initialize mini to current value
	mini = analogRead(rot) >> 4;
	
	//populate map with read values - exit on serial input or calibration flag dropping
	while(digitalRead(interPin)){
		//read values
		currVal = analogRead(rot)>>4;
		linVal = analogRead(lin)>>4;
		
		//store values
		potVal[currVal] = linVal;

	}
	
	//calculate min/max values and err values
	calcRange();
	
	Serial.print(mini); Serial.print(" = mini, maxi = "); Serial.println(maxi);
	Serial.println("Dump of potVal:");
	for(unsigned int i = 0; i < 256; i++){
		Serial.print(i); Serial.print(" : "); Serial.println(potVal[i]);
	}
	digitalWrite(PC13, 0);
}

inline void Pedal::calcRange(){
	uint8_t lini=255, laxi=0;	//linear pot min and max
	for(unsigned int i = 0; i < 256; i++){
		//set mini and maxi
		if(potVal[i]){
			if(mini > i) mini = i;
			else if(maxi < i) maxi = i;
			if(lini > potVal[i]) lini = potVal[i];
			else if(laxi < potVal[i]) laxi = potVal[i];
		}
	}
	
	//initialize error: error is 10% of the range between maxi and mini
	err = (laxi-lini)/10;
}

bool Pedal::calibrate(int interPin, SdFile outFile){
	calibrate(interPin);
	if(!outFile.isOpen()){
		return false;
	}
	outFile.rewind();
	if(outFile.write(potVal, 256) != 256 || !outFile.sync()) return false;
	return true;
}

bool Pedal::load(SdFile inFile){
	if(!inFile.isOpen()){
		return false;
	}
	inFile.rewind();
	if(inFile.read(potVal, 256) != 256) return false;
	
	//calculate range etc based on loaded data
	calcRange();
	
	return true;
}

short Pedal::read(){
	//read and store the values
	uint8_t currVal = analogRead(rot)>>4;
	uint8_t linVal = analogRead(lin)>>4;
	
	//if errFlag is set, return 0 and print err
	if(errFlag){
		return -1;
	}
	
	//check if there's an error
	//Serial.print(Timer2.getCount()); Serial.print(" ");
	if(check(currVal, linVal)){
		
		//disable timer if its running
		if(flag){
			Timer2.pause();
			Timer2.refresh();
		}
		//return pedal position, mapped between 0 and 255
		return constrain(map(currVal, mini+dZone[0], maxi-dZone[1], 0, 255), 0, 255);
	}
	
	//error
	else{
		Serial.print("err!");
		flag = true;
		Timer2.resume();
		return constrain(map(currVal, mini+dZone[0], maxi-dZone[1], 0, 255), 0, 255);
	}
}
