/*
	Pedal.cpp : a library to control the pedal for an electric car
	maps linear pot to rotary pot during calibration, then run checks to see if map fails
	
	rotary pot: clarostat hrs100swab090
	linear pot: ???
	
	Chris Davies, sept 2018
*/

#include "Arduino.h"
#include "pedal.h"

volatile bool errFlag;	//if the pedal has a read error > 100ms

void setErr(){
	errFlag = true;
}

Pedal::Pedal(int rotaryPin, int linearPin){
	delay(1000);
	Serial.println("value init");
	//value initialization
	rot = rotaryPin;
	lin = linearPin;
	mini = -1;
	maxi = 0;
	err = 0;
	dZone[0] = 10;
	dZone[1] = 10;
	for(unsigned int i = 0; i < 256; i++) potVal[i] = 0;
	
	Serial.println("now doing timer init");
	
	//100ms timer for error
	Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
	Timer2.setPeriod(100000);	// 100Hz in microseconds
	Timer2.attachCompare1Interrupt(setErr);
	Timer2.pause();
	Serial.println("init done");
}

//returns true if error is within acceptable levels
inline bool Pedal::check(byte rotVal, byte linVal){
	return (potVal[rotVal] < linVal+err && potVal[rotVal] > linVal-err);
}

void Pedal::calibrate(int interPin){
	
	byte currVal, linVal;
	byte lini, laxi;	//linear pot min and max
	//initialize mini to current value
	mini = analogRead(rot) >> 4;
	lini = analogRead(lin) >> 4;
	
	//populate map with read values - exit on serial input or calibration flag dropping
	while(digitalRead(interPin) && !Serial.available()){
		//read values
		currVal = analogRead(rot)>>4;
		linVal = analogRead(lin)>>4;
		
		//store values
		potVal[currVal] = linVal;
		
		//set mini and maxi
		if(mini > currVal) mini = currVal;
		else if(maxi < currVal) maxi = currVal;
		if(lini > linVal) lini = linVal;
		else if(laxi < linVal) laxi = linVal;
	}
	
	//initialize error: error is 10% of the range between maxi and mini
	err = (laxi-lini)/10;
	Serial.print(mini); Serial.print(" = mini, maxi = "); Serial.println(maxi);
}

short Pedal::read(){
	//read and store the values
	byte currVal = analogRead(rot)>>4;
	byte linVal = analogRead(lin)>>4;
	
	//if errFlag is set, return 0 and print err
	if(errFlag){
		return -1;
	}
	
	//check if there's an error
	Serial.print(Timer2.getCount()); Serial.print(" ");
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
