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
	error = 0;
	for(unsigned int i = 0; i < 256; i++) potVal[i] = 0;
}

inline bool Pedal::check(byte rotVal, byte linVal){
	return (potVal[rotVal] < linVal+err && potVal[rotVal] > linVal-err);
}

void Pedal::calibrate(){
	byte currVal;
	bool flag =1;
	while(!Serial.available()){
		//Store the value of 'rot' into mini just on the first cycle
		mini = flag == 1 ? analogRead(rot) >> 4: ;
		flag =0;
		currVal = analogRead(rot)>>4;
		potVal[currVal] = analogRead(lin)>>4;
		if(mini > currVal) mini = currVal;
		else if(maxi < currVal) maxi = currVal;
	}
	error = maxi-mini/10	//error is 10% of the range between maxi and mini
	Serial.print(mini); Serial.print(" = mini, maxi = "); Serial.println(maxi);
}

byte Pedal::read(){
	byte currVal = analogRead(rot)>>4;
	byte linVal = analogRead(lin)>>4;
	if(check(currVal, linVal)){
		return map(currVal, mini, maxi, 0, 255);
	}
	else{
		//Maybe a flashing LED to indicate a problem?
	}
	//error handling here
	Serial.print("Dead!");
}
