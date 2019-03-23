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
		this.x = window.innerWidth
		this.y = window.innerHeight
		this.color = '#fae3bf'
	}

	draw() {
		background_context.rect(0, 0, this.x, this.y)
		background_context.fillStyle = this.color
		background_context.fill()
	}
}

let arena = new Arena()
arena.draw()

class RockyTerrain {
	constructor() {
		this.x = window.innerWidth * (0.65 / 4)
		this.x_offset = window.innerWidth * (0.6 / 4)
		this.y = window.innerWidth /  2

		this.color = '#d2bb9a'
	}

	draw() {
		context.rect(this.x_offset, 0, this.x, this.y)
		context.fillStyle = this.color
		context.fill()
	}
}

let rockyTerrain = new RockyTerrain()
rockyTerrain.draw()