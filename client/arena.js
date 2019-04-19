const ARENA_X = 4
const ARENA_Y = 2

const ROCKY_TERRAIN_OFFSET = 0.6
const ROCKY_TERRAIN_X = 0.65

const OBSTACLE_X = 0.2
const OBSTACLE_Y = 0.5

const DESTINATION_RADIUS = 0.09

let background = document.getElementById('background')
let background_context = background.getContext('2d')
let foreground = document.getElementById('foreground')
let foreground_context = foreground.getContext('2d')


const SAND_COLORS = [
	'#F9E3C1',
	'#CFBCAD',
	'#CFC8BF',
	'#DFC5AD',
	'#BFA783',
]

function hexToRgb(hex) {
    var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
        r: parseInt(result[1], 16),
        g: parseInt(result[2], 16),
        b: parseInt(result[3], 16)
    } : null;
}

class Arena {
	constructor() {
		this.colors = SAND_COLORS
	}

	resize(width, height) {
		this.width = width
		this.height = height
	}

	draw() {

		let imageData = background_context.getImageData(0, 0, this.width, this.height)

		for (let i = 0; i < imageData.data.length / 4; i++) {

			let hex = this.colors[Math.floor(Math.random() * this.colors.length)]
			let rgb = hexToRgb(hex)

			imageData.data[4*i] = rgb.r;    // RED (0-255)
			imageData.data[4*i+1] = rgb.g;    // GREEN (0-255)
			imageData.data[4*i+2] = rgb.b;    // BLUE (0-255)
			imageData.data[4*i+3] = 255;  // APLHA (0-255)
		}

		background_context.putImageData(imageData,0,0);
		// background_context.fillRect(0, 0, this.width, this.height)
	}
}

class RockyTerrain {
	constructor(canvas_width, canvas_height) {
		this.color = '#d0a9ae'
	}

	resize(width, height) {
		this.x = width * (ROCKY_TERRAIN_OFFSET / ARENA_X)
		this.y = 0
		this.width = width * (ROCKY_TERRAIN_X / ARENA_X)
		this.height = height
	}

	draw() {
		foreground_context.fillStyle = this.color
		foreground_context.fillRect(this.x, this.y, this.width, this.height)
	}
}


class Obstacle {
	constructor(x, y) {
		this.actual_x = x
		this.actual_y = y

		this.color = '#556B2F'
	}

	resize(width, height) {
		this.x = width * (this.actual_x / ARENA_X)
		this.y = height * ((2 - this.actual_y) / ARENA_Y)
		this.width = width * (OBSTACLE_X / ARENA_X)
		this.height = height * (OBSTACLE_Y / ARENA_Y)
	}

	draw() {
		foreground_context.fillStyle = this.color
		foreground_context.fillRect(this.x, this.y, this.width, this.height)
	}
}


class Destination {
	constructor(x, y) {
		this.actual_x = x
		this.actual_y = y

		this.color = 'blue'
	}

	resize(width, height) {
		this.x = width * (this.actual_x / ARENA_X)
		this.y = height * ((2 - this.actual_y) / ARENA_Y)
		this.radius = width * (DESTINATION_RADIUS / ARENA_X)
	}

	draw() {
		foreground_context.beginPath()
		foreground_context.arc(this.x, this.y, this.radius, 0, Math.PI * 2, false)
		foreground_context.strokeStyle = this.color
		foreground_context.stroke()
		foreground_context.closePath()
	}
}

class Canvas {
	constructor() {
		this.arena = new Arena()
		this.rockyTerrain = new RockyTerrain()
		this.obstacles = undefined
		this.destination = undefined
		this.osv_frames = undefined
	}

	width() {
		return $('canvas').first().parent().width()
	}

	height() {
		return this.width() / 2
	}

	draw() {

		background.width = this.width()
		background.height = this.height()
		foreground.width = this.width()
		foreground.height = this.height()

		this.elements().map(element => {
			if (element !== undefined) {
				element.resize(this.width(), this.height())
				element.draw()
			}
		})
	}

	elements() {
		return [this.arena, this.rockyTerrain, this.obstacles, this.destination, this.osv].flat()
	}

}


let canvas = new Canvas()
canvas.draw()

$(window).resize(() => {
	canvas.draw()
})

