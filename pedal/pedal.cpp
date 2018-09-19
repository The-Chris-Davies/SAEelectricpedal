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
	rot = rotaryPin;
	lin = linearPin;
	mini = -1;
	maxi = 0;
	err = 0;
	dZone = {10, 10};
	for(unsigned int i = 0; i < 256; i++) potVal[i] = 0;
	
	//100ms timer for error
	tim(2);
	tim.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
	tim.setPeriod(100000);	// 10Hz in microseconds
	tim.attachCompare1Interrupt(printerrupt);
	tim.pause();
}

inline bool Pedal::check(byte rotVal, byte linVal){
	return (potVal[rotVal] < linVal+err && potVal[rotVal] > linVal-err);
}

void Pedal::calibrate(){
	byte currVal;
	mini = analogRead(rot) >> 4;
	while(!Serial.available()){
		currVal = analogRead(rot)>>4;
		potVal[currVal] = analogRead(lin)>>4;
		if(mini > currVal) mini = currVal;
		else if(maxi < currVal) maxi = currVal;
	}
	mini -= dZone[0];
	maxi -= dZone[1];
	err = maxi-mini/10;	//error is 10% of the range between maxi and mini
	Serial.print(mini); Serial.print(" = mini, maxi = "); Serial.println(maxi);
}

byte Pedal::read(){
	//read and store the vals
	byte currVal = analogRead(rot)>>4;
	byte linVal = analogRead(lin)>>4;
	
	//check if there's an error
	if(check(currVal, linVal)){
		
		//disable timer if its running
		if(flag){
			tim.pause();
			tim.refresh();
		}
		
		if(currVal < mini) return 0;
		else if(currVal > maxi) return 255;
		return map(currVal, mini, maxi, 0, 255);
	}
	
	//error
	else{
		flag = true;
		tim.resume();
	}
}

void Pedal::printerrupt(){
	Serial.println("Error raised!");
}
