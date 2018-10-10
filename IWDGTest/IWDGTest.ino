/*
Testing watchdog timer on STM32

*/


void setup() {
	Serial.begin(19200);
	delay(2000);	//delay to set up serial
	Serial.println("Starting Up...");

	//watchdog timer is 40kHz:
	//prescaler value of 32 (from IWDG_PRE_32) gives approx. 1 ms per counter value.

	iwdg_init(IWDG_PRE_32, 100)	//init watchdog (very roughly 100ms)
}

bool feeding = false;
char kar;
void loop() {
	if(Serial.available()){
		kar = Serial.read();
		if(kar == 'r'){
			Serial.println("running");
			feeding = true;
		}
		else if(kar == 's'){
			Serial.println("stopping");
			feeding = false;
		}
	}
	if(feeding)
		iwdg_feed();	//reset watchdog
	delay(10);
	Serial.println(millis());
}

void printerrupt(void) {
	Serial.println(millis());
	toggleLED();
} 

void toggleLED(){
	digitalWrite(PC13, !digitalRead(PC13));
}
