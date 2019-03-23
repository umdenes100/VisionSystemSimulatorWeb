$(document).ready(() => {

	resizeArena()

	const connection = new WebSocket("ws://127.0.0.1:8888/")

	connection.onopen = () => {
		console.log('OPEN')
		connection.send(JSON.stringify(request))
		connection.send(JSON.stringify(request))
		connection.send(JSON.stringify(bad_request))
	}

	connection.onerror = error => {
	    console.log('WebSocket Error.')
	}

	connection.onmessage = message => {

		message = JSON.parse(message.data)
		console.log(message)
		if (message.type == 'randomization') {

			// console.log(message.destination.x * ARENA_WIDTH_SCALE)
			// console.log(message.destination.y * ARENA_HEIGHT_SCALE)
			// console.log(DESTINATION_RADIUS * ARENA_WIDTH_SCALE)

			// $('canvas').drawArc({
			//   layer: true,
			//   strokeStyle: 'blue',
			//   strokeWidth: 2,
			//   x: message.destination.x * ARENA_WIDTH_SCALE, 
			//   y: message.destination.y * ARENA_HEIGHT_SCALE,
			//   radius: DESTINATION_RADIUS
			// })

		// 	$canvas.drawRect({
		// 	  layer: true,
		// 	  fillStyle: '#957e5a',
		// 	  x: message.osv.x * ARENA_SCALE, y: (message.osv.y - osv_height) * ARENA_SCALE,
		// 	  fromCenter: false,
		// 	  width: osv_width * ARENA_SCALE,
		// 	  height: osv_height * ARENA_SCALE
		// 	})

		// 	message.obstacles.forEach(obstacle => {
		// 		$canvas.drawRect({
		// 		  layer: true,
		// 		  fillStyle: '#957e5a',
		// 		  x: obstacle.x * ARENA_SCALE, y: (obstacle.y - OBSTACLE_HEIGHT) * ARENA_SCALE,
		// 		  fromCenter: false,
		// 		  width: OBSTACLE_WIDTH * ARENA_SCALE,
		// 		  height: OBSTACLE_HEIGHT * ARENA_SCALE
		// 		})
		// 	})

		// 	$canvas.drawLayers()

		} else {
			console.log('Unimplemented')
		}
	}

	connection.onclose = () => {
		console.log('Failed')
	}




	let editor = CodeMirror(document.getElementById('code-editor'), {
		value: 'void setup() {\n\n}\n\nvoid loop() {\n\n}',
		theme: 'midnight',
		indentUnit: 4,
		firstLineNumber: 0,
		lineNumbers: true,
		lineWrapping: true,
		indentWithTabs: true,
	})

})


