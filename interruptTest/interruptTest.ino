/*
Testing timer-based interrupt calls on STM32

*/

HardwareTimer tim(2);	//timer 2

void setup() {
	Serial.begin(19200); 
	tim.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
	tim.setPeriod(1000000);	// 1s in microseconds
	tim.attachCompare1Interrupt(printerrupt);
	pinMode(PC13, OUTPUT);
}

bool running = false;
char kar;
void loop() {
	if(Serial.available()){
		 kar = Serial.read();
		if(kar == 'r'){
			Serial.println("running");
			tim.resume();
		}
		else if(kar == 's'){
			Serial.println("stopping");
			tim.pause();
		}
		if(kar == 'd'){
			tim.refresh();
		}
	}
}

void printerrupt(void) {
	Serial.println(millis());
	toggleLED();
} 

void toggleLED(){
	digitalWrite(PC13, !digitalRead(PC13));
}
