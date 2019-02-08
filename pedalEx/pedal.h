/*
	Pedal.cpp : a library to control the pedal for an electric car
	maps linear pot to rotary pot during calibration, then run checks to see if map fails
	
	rotary pot: clarostat hrs100swab090
	linear pot: ???
	
	Chris Davies, sept 2018
*/

#ifndef POTLIB
#define POTLIB

#include "Arduino.h"
#include <SPI.h>
#include "SdFat.h"

class Pedal{
	private:
		uint8_t potVal[256];		//the map between the values. Index is rotary, value is linear.
		int16_t mini, maxi, err;	//minimum, maximum, and error values for the rotary pot
		uint8_t dZone[2];			//the dead zone at the top/bottom of the pedal.
		bool flag;				//whether timer is running
		int rot, lin;					//the pins to test
		inline bool check(uint8_t, uint8_t);	//check if the uint8_t is ok
		inline void calcRange();	//calculate the error and max/min values for calibration
	
	public:
		Pedal();
		Pedal(int, int);
		void setPins(int, int);	//set input pins
		void calibrate(int);	//fill in potVal
		bool calibrate(int, SdFile);	//fill in potVal and save to SD
		bool load(SdFile);		//load values from file on SD card
		short read();			//test whether the values coorelate, and return throttle value (0-255)
};

#endif
