$(document).ready(() => {

	// const connection = new WebSocket("ws://18.191.246.34:8888/")
	const connection = new WebSocket("ws://127.0.0.1:8888/")

	connection.onopen = () => {
		console.log('OPEN')
		connection.send(JSON.stringify(request))
	}

	connection.onerror = error => {
	    console.log('WebSocket Error.')
	}

	connection.onmessage = message => {

		message = JSON.parse(message.data)
		console.log(message)
		if (message.type == 'randomization') {

			message.obstacles.map(obstacle => {
				let obs = new Obstacle(obstacle.x, obstacle.y)
				obs.draw()
			})

			let destination = new Destination(message.destination.x, message.destination.y)
			destination.draw()
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


