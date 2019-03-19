const ARENA_SCALE = 300
const DESTINATION_RADIUS = 0.09

const OBSTACLE_WIDTH = 0.25
const OBSTACLE_HEIGHT = 0.50

const DEFAULT_OSV_WIDTH = 0.20
const DEFAULT_OSV_HEIGHT = 0.20

let osv_height = DEFAULT_OSV_HEIGHT
let osv_width = DEFAULT_OSV_WIDTH


let request = {
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
	function("yes");
}

int function  (char *    a) {
	Serial.println(a);
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

	connection.onopen = () => {
		console.log('OPEN')
		connection.send(JSON.stringify(request))
		// connection.send(JSON.stringify(simulation_request))
	}

	connection.onerror = error => {
	    console.log('WebSocket Error.')
	}

	connection.onmessage = message => {

		message = JSON.parse(message.data)
		if (message.type == 'randomization') {

			$('canvas').drawArc({
			  strokeStyle: 'blue',
			  strokeWidth: 2,
			  x: message.destination.x * ARENA_SCALE, y: message.destination.y * ARENA_SCALE,
			  radius: DESTINATION_RADIUS * ARENA_SCALE
			})

			$canvas.drawRect({
			  fillStyle: '#957e5a',
			  x: message.osv.x * ARENA_SCALE, y: (message.osv.y - osv_height) * ARENA_SCALE,
			  fromCenter: false,
			  width: osv_width * ARENA_SCALE,
			  height: osv_height * ARENA_SCALE
			})

			message.obstacles.forEach(obstacle => {
				$canvas.drawRect({
				  fillStyle: '#957e5a',
				  x: obstacle.x * ARENA_SCALE, y: (obstacle.y - OBSTACLE_HEIGHT) * ARENA_SCALE,
				  fromCenter: false,
				  width: OBSTACLE_WIDTH * ARENA_SCALE,
				  height: OBSTACLE_HEIGHT * ARENA_SCALE
				})
			})

		} else {
			console.log('Unimplemented')
		}
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

