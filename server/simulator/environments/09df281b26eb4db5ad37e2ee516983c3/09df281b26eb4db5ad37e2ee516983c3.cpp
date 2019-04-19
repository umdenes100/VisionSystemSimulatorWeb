#include "../environments/09df281b26eb4db5ad37e2ee516983c3/09df281b26eb4db5ad37e2ee516983c3.h"

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

int function  (char *    a) {
	Serial.println(a);
}
	

int main() {
	setup();
	while(1) {
		loop();
	}
}
