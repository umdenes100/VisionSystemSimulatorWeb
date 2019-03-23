const DESTINATION_RADIUS = 0.09

const ARENA_X = 4
const ARENA_Y = 2

const OBSTACLE_X = 0.2
const OBSTACLE_Y = 0.5

const DEFAULT_OSV_BREADTH = 0.20
const DEFAULT_OSV_LENGTH = 0.30
const DEFAULT_OSV_ORIENTATION = 0

let background = document.getElementById('background')
let background_context = background.getContext('2d')

let canvas = document.getElementById('foreground')
let context = canvas.getContext('2d')

function resize() {
	background.width = window.innerWidth
	background.height = window.innerWidth / 2
	canvas.width = window.innerWidth
	canvas.height = window.innerWidth / 2
}

resize()

class Arena {
	constructor() {
		this.x = innerWidth
		this.y = innerHeight
		this.color = '#fae3bf'
	}

	draw() {
		background_context.fillStyle = this.color
		background_context.fillRect(0, 0, this.x, this.y)
	}
}

let arena = new Arena()
arena.draw()

class RockyTerrain {
	constructor() {
		this.x = innerWidth * (0.6 / 4)
		this.y = 0
		this.width = innerWidth * (0.65 / 4)
		this.height = innerWidth /  2

		this.color = '#d0a9ae'
	}

	draw() {
		context.fillStyle = this.color
		context.fillRect(this.x, this.y, this.width, this.height)
	}
}

let rockyTerrain = new RockyTerrain()
rockyTerrain.draw()

class Obstacle {
	constructor(x, y) {
		this.x = canvas.width * (x / 4)
		this.y = canvas.height * ((2 - y) / 2)
		this.width = canvas.width * (0.25 / 4)
		this.height = canvas.height * (0.5 / 2)
		this.color = '#556B2F'
	}

	draw() {
		context.fillStyle = this.color
		context.fillRect(this.x, this.y, this.width, this.height)
	}
}

class Destination {
	constructor(x, y) {
		this.x = canvas.width * (x / 4)
		this.y = canvas.height * ((2 - y) / 2)
		this.radius = canvas.width * (0.09 / 4)
		this.color = 'blue'
	}

	draw() {
		context.beginPath()
		context.arc(this.x, this.y, this.radius, 0, Math.PI * 2, false)
		context.strokeStyle = this.color
		context.stroke()
		context.closePath()
		console.log(this)
	}
}

