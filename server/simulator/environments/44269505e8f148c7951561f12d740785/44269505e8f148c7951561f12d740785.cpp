#include "../environments/44269505e8f148c7951561f12d740785/44269505e8f148c7951561f12d740785.h"

#include "Enes100.h"
#include "Tank.h"

int wow = 6;

void setup() {
	pinMode(16, OUTPUT);
}

void loop() {
	digitalWrite(3, HIGH);
	// function("yes");
}

int function  (char *    a) {
	Serial.println(a);
}
	

int main() {
	setup();
	while(1) {
		loop();
	}
}
