$(document).ready(() => {

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


