/*
just reads the values from the specified pins and writes them to serial.

*/
#include "pedal.h"

#define ROTARYPIN PB1
#define LINEARPIN PB0

Pedal ped(ROTARYPIN, LINEARPIN);
byte lin, rot;

void setup() {
	// initialize digital pin ROTARYPIN1 as an output.
	pinMode(ROTARYPIN0, INPUT);
	pinMode(PC13, OUTPUT);
	Serial.begin(9600);
	
	Serial.println("test start");
	digitalWrite(PC13, HIGH);
	Serial.println("initialized");
	digitalWrite(PC13, LOW);
	delay(500);
	digitalWrite(PC13, HIGH);
	delay(500);
}

// the loop function runs over and over again forever
void loop() {
	rot = analogRead(ROTARYPIN)>>4;
	lin = analogRead(LINEARPIN)>>4;
	Serial.print(rot); Serial.print(" : "); Serial.println(lin);
	Serial.print("Pedal::read() value: "); Serial.println(ped.read());
}
