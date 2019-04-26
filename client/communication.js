let randomization = undefined
let simulation = undefined
let editor = undefined

$(document).ready(() => {

	// const connection = new WebSocket("ws://18.191.246.34:8888/")
	const connection = new WebSocket("ws://127.0.0.1:8888/")

	connection.onopen = () => {
		console.log('OPEN')
		connection.send(JSON.stringify(randomization_request))
		connection.send(JSON.stringify(simulation_request))
	}

	connection.onerror = error => {
	    console.log(`WebSocket Error.: ${error}`)
	}

	connection.onmessage = message => {

		message = JSON.parse(message.data)
		console.log(message)
		if (message.type === 'randomization') {

			let $textarea = $('#terminal');
			$textarea.append('Updated randomization.\n')
			let last_thousand_messages = $textarea.val().split('\n').slice(-1000).join('\n')
			$textarea.empty()
			$textarea.append(last_thousand_messages)
			$textarea.scrollTop($textarea[0].scrollHeight);

			randomization = message

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

	editor = CodeMirror(document.getElementById('code-editor'), {
		value: 'void setup() {\n\n}\n\nvoid loop() {\n\n}',
		// theme: 'night',
		mode: "text/x-c++src",
		indentUnit: 4,
		indentWithTabs: true,
		firstLineNumber: 0,
		extraKeys: {"Ctrl-Space": "autocomplete"},
		matchBrackets: true,
		lineNumbers: true,
		lineWrapping: true,
	})

	$('#randomize').on('click', () => {
		connection.send(JSON.stringify(randomization_request))
	})

	$('#simulate').on('click', () => {

		let r = randomization
		r.osv.height = 0.3
		r.osv.width = 0.3

		let simulation_request = JSON.stringify({
			type: 'simulation',
			code: editor.getDoc().getValue(),
			randomization: r,
			distance_sensors: [0, 2],
		})

		connection.send(simulation_request)
	})

})


