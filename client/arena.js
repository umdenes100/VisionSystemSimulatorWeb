const DESTINATION_RADIUS = 0.09

const ARENA_X = 4
const ARENA_Y = 2

const OBSTACLE_X = 0.2
const OBSTACLE_Y = 0.5

const DEFAULT_OSV_BREADTH = 0.20
const DEFAULT_OSV_LENGTH = 0.30
const DEFAULT_OSV_ORIENTATION = 0

const ROCKY_TERRAIN_OFFSET_X = 0.6
const ROCKY_TERRAIN_X = 0.65


function xScale(dim) {
	return $('canvas').width() * (dim / ARENA_X)
}


function yScale(dim) {
	return $('canvas').height() * (dim / ARENA_Y)
}

function drawArena() {
	$('canvas').drawRect({
	  fillStyle: '#FAE3BF',
	  x: 0, y: 0,
	  fromCenter: false,
	  width: xScale(ARENA_X),
	  height: yScale(ARENA_Y),
	  fromCenter: false
	})
}


$(window).resize(resizeArena)

function resizeArena() {
	const $canvas = $('canvas')
	$canvas.height($canvas.width() / 2)

	console.log($canvas.width())
	console.log($canvas.height())

	drawArena()

	$canvas.drawRect({
	  fillStyle: '#d2bb9b',
	  x: xScale(ROCKY_TERRAIN_OFFSET_X), y: 0,
	  fromCenter: false,
	  width: xScale(ROCKY_TERRAIN_X),
	  height: yScale(ARENA_Y),
	  fromCenter: false,
	})

	console.log(ROCKY_TERRAIN_X)
	console.log(xScale(ROCKY_TERRAIN_X))

}