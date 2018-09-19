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

class Pedal{
	private:
		byte potVal[255];		//the map between the values. Index is rotary, value is linear.
		byte mini, maxi, err;	//minimum, maximum, and error values for the rotary pot
		byte dZone[2];			//the dead zone at the top/bottom of the pedal.
		bool flag;				//flag for error timer
		int rot, lin;					//the pins to test
		inline bool check(byte, byte);	//check if the byte is ok
	
	public:
		Pedal(int, int);
		void calibrate();			//fill in potVal
		byte read();					//test whether the values coorelate, and return throttle value (0-255)
		
};

#endif
