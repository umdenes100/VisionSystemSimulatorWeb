let connection = new WebSocket("ws://127.0.0.1:8888/")

let request = {
    type: 'randomization'
}

console.log(request)

connection.onopen = () => {
	console.log('OPEN')
	connection.send(JSON.stringify(request))
}

connection.onerror = error => {
    console.log('WebSocket Error.')
}

connection.onmessage = message => {
	console.log(message.data)
}

connection.onclose = () => {
	console.log('Failed')
}
