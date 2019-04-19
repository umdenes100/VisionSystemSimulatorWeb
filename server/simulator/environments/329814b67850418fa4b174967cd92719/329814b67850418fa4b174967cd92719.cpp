#include "../environments/329814b67850418fa4b174967cd92719/329814b67850418fa4b174967cd92719.h"

#include "Enes100.h"
#include "Tank.h"

int wow = 6

void setup() {
	pinMode(16, OUTPUT);
}

void loop() {
	digitalWrite(3, HIGH);
	function("yes");
}

int function (char *  a) {
	Serial.println(a);
}
	

int main() {
	setup();
	while(1) {
		loop();
	}
}
