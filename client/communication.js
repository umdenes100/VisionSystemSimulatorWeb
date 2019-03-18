let request = {
    type: 'randomization'
}

let simulation_request = {
	code: `
#include <iostream>
#include "Enes100.h"
#include "Tank.h"

int wow = 3;

char *string = "wowwowowowo";

Tank tank;

void setup() {
	pinMode(3, OUTPUT);
	digitalWrite(3, LOW);
	pinMode(4, INPUT);
	digitalRead(4);
	Serial.begin(9600);
	tank.begin();
	delay(20);
}

void loop() {
	Serial.println("wow");
	std::cout << "hello";
	Enes100.updateLocation();
	tank.setLeftMotorPWM(200);
	for(int i = 0; i < 10; i++) {
		Serial.println(string);
	}
}

void function( int wow, char c) {
	return;
}
	`,
	randomization: {
		osv: {
				x: 0.35,
				y: 0.7,
				theta: -3.1415901184082031
		},
		obstacles: [
			{
				x: 1.5,
				y: 1.25
			}, {
				x: 2.05,
				y: 1.8999999761581421
			}, {
				x: 2.6,
				y: 0.60000002384185791
			}
		],
		destination:  {
			x: 3.4056000709533691,
			y: 0.49000000953674316
		}
	}
}


$(document).ready(() => {
	let connection = new WebSocket("ws://127.0.0.1:8888/")

	console.log(request)

	connection.onopen = () => {
		console.log('OPEN')
		connection.send(JSON.stringify(request))
	}

	connection.onerror = error => {
	    console.log('WebSocket Error.')
	}

	connection.onmessage = message => {
		
		console.log(JSON.parse(message.data))
	}

	connection.onclose = () => {
		console.log('Failed')
	}

	const $canvas = $('#canvas')

	$canvas.drawRect({
	  fillStyle: '#d2bb9b',
	  x: 200, y: 0,
	  fromCenter: false,
	  width: 200,
	  height: 600
	})
})

