$(document).ready(() => {

	const connection = new WebSocket("ws://18.191.246.34:8888/")
	// const connection = new WebSocket("ws://127.0.0.1:8888/")

	connection.onopen = () => {
		console.log('OPEN')
		connection.send(randomization_request)
	}

	connection.onerror = error => {
	    console.log('WebSocket Error.')
	}

	connection.onmessage = message => {

		message = JSON.parse(message.data)
		console.log(message)
		if (message.type == 'randomization') {
			canvas.obstacles = message.obstacles.map(obstacle => new Obstacle(obstacle.x, obstacle.y))
			canvas.destination = new Destination(message.destination.x, message.destination.y)
			canvas.draw()
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


	$('#randomize').on('click', () => {
		connection.send(randomization_request)
	})

})


