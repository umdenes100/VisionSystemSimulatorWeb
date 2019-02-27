let socket = new WebSocket("ws://0.0.0.0:8888/");

connection.onopen = () => {
	console.log('OPEN')
}

connection.onerror = error => {
    console.log('WebSocket Error.')
}

connection.onmessage = () => {
	console.log('Bananaa')
}

connection.onclose = () => {
	console.log('Failed')
}
