/*
just reads the values from the specified pins and writes them to serial.

*/

#define ROTARYPIN PB1
#define LINEARPIN PB0

byte lin, rot;

void setup() {
	pinMode(PC13, OUTPUT);
	Serial.begin(9600);
	
	//Test start
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
	//Test main loop
	rot = analogRead(ROTARYPIN)>>4;
	lin = analogRead(LINEARPIN)>>4;
	Serial.print(rot); Serial.print(" : "); Serial.println(lin);
}
