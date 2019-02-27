let socket = new WebSocket("127.0.0.1", 8080);

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
