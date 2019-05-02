let randomization = undefined
let simulation = undefined
let editor = undefined

const SERVER_URL = 'http://18.191.246.34:8888'
const LOCAL_URL = 'http://127.0.0.1:8888'


function request_randomization() {
    let request = { type: 'randomization' }
    $.get(SERVER_URL, request, data => {
        let canvasses = [pcanvas, canvas]

        canvasses.map(canv => {
            canv.obstacles = data.obstacles.map(obstacle => new Obstacle(obstacle.x, obstacle.y))
            canv.destination = new Destination(data.destination.x, data.destination.y)
            canv.draw()
        })
    })
}

$(document).ready(() => {


	request_randomization()



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

	$('#randomize').on('click', request_randomization)

	$('#simulate').on('click', () => {

		let r = randomization
		r.osv.height = 0.3
		r.osv.width = 0.3

		let simulation_request = {
			type: 'simulation',
			code: editor.getDoc().getValue(),
			randomization: r,
			distance_sensors: [0, 2],
		}
		$.get(SERVER_URL, simulation_request, data => {
			console.log(data)
		})
	})

})


