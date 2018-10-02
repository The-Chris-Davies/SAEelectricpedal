/*
just reads the values from the specified pins and writes them to serial.

*/

#define ROTARYPIN PA1
#define LINEARPIN PA0

byte lin, rot;

void setup() {
	pinMode(PC13, OUTPUT);
	Serial.begin(9600);
	pinMode(PB13, INPUT);	//PB13: Calibration mode
	pinMode(PB14, INPUT);	//PB14: Run mode
	//Test start
	delay(500);
}

// the loop function runs over and over again forever
void loop() {
	//Test main loop
	rot = analogRead(ROTARYPIN)>>4;
	lin = analogRead(LINEARPIN)>>4;
	Serial.print(rot); Serial.print(" : "); Serial.println(lin);
	Serial.print(digitalRead(PB13)); Serial.print(" : "); Serial.println(digitalRead(PB14));
}
