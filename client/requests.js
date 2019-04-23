let bad_request = 'This is a terribly formatted request.'

let randomization_request = {
    type: 'randomization'
}

let simulation_request = {
	type: 'simulation',
	code: `
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
	`,
	randomization: {
		osv: {
				x: 0.35,
				y: 0.7,
				theta: -3.1415901184082031,
				height: 0.3,
				width: 0.3,
		},
		obstacles: [
			{
				x: 1.5,
				y: 1.25,
				width: 0.2,
				height: 0.5,
			}, {
				x: 2.05,
				y: 1.8999999761581421,
				width: 0.2,
				height: 0.5,
			}, {
				x: 2.6,
				y: 0.60000002384185791,
				width: 0.2,
				height: 0.5,
			}
		],
		destination:  {
			x: 3.4056000709533691,
			y: 0.49000000953674316
		}
	},
	distance_sensors: [0, 2],
}
