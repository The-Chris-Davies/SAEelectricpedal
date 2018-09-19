/*
	Pedal.cpp : a library to control the pedal for an electric car
	maps linear pot to rotary pot during calibration, then run checks to see if map fails
	
	rotary pot: clarostat hrs100swab090
	linear pot: ???
	
	Chris Davies, sept 2018
*/

#include "Arduino.h"
#include "pedal.h"

Pedal::Pedal(int rotaryPin, int linearPin){
	//value initialization
	rot = rotaryPin;
	lin = linearPin;
	mini = -1;
	maxi = 0;
	err = 0;
	dZone[0] = 10;
	dZone[1] = 10;
	for(unsigned int i = 0; i < 256; i++) potVal[i] = 0;
	
	//100ms timer for error
	Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
	Timer2.setPeriod(100000);	// 10Hz in microseconds
	Timer2.attachCompare1Interrupt([](){Serial.println("Error!");});
	Timer2.pause();
}

//returns true if error is within acceptable levels
inline bool Pedal::check(byte rotVal, byte linVal){
	return (potVal[rotVal] < linVal+err && potVal[rotVal] > linVal-err);
}

void Pedal::calibrate(){
	byte currVal;
	
	//initialize mini to current value
	mini = analogRead(rot) >> 4;
	
	//populate map with read values
	while(!Serial.available()){
		currVal = analogRead(rot)>>4;
		potVal[currVal] = analogRead(lin)>>4;
		if(mini > currVal) mini = currVal;
		else if(maxi < currVal) maxi = currVal;
	}
	
	//add dead zone
	mini -= dZone[0];
	maxi -= dZone[1];
	
	//initialize error: error is 10% of the range between maxi and mini
	err = maxi-mini/10;
	Serial.print(mini); Serial.print(" = mini, maxi = "); Serial.println(maxi);
}

byte Pedal::read(){
	//read and store the values
	byte currVal = analogRead(rot)>>4;
	byte linVal = analogRead(lin)>>4;
	
	//check if there's an error
	if(check(currVal, linVal)){
		
		//disable timer if its running
		if(flag){
			Timer2.pause();
			Timer2.refresh();
		}
		//return pedal position, mapped between 0 and 255
		return constrain(map(currVal, mini, maxi, 0, 255), 0, 255);
	}
	
	//error
	else{
		flag = true;
		Timer2.resume();
	}
}
