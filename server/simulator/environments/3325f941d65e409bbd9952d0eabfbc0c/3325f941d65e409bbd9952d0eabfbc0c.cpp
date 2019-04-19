#include "../environments/3325f941d65e409bbd9952d0eabfbc0c/3325f941d65e409bbd9952d0eabfbc0c.h"

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
