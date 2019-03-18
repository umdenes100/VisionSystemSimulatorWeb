from asyncio import create_subprocess_exec, subprocess, get_event_loop
import websockets
import uuid
import json

HOST = '0.0.0.0'
PORT = 8888

RANDOMIZE = './randomization/randomize'
SIMULATE = './simulator/simulator/simulate'

async def process_command(command, data=None):
	process = await create_subprocess_exec(*command.split(), 
										   stdin=subprocess.PIPE, 
										   stdout=subprocess.PIPE)
	if data is None:
		stdout, stderr = await process.communicate()
	else:
		stdout, stderr = await process.communicate(input=bytes(str(data), encoding='utf-8'))

	print(f'Error: {stderr}')
	print(f'Return Code: {process.returncode}')
	print(f'General Process Info: {vars(process)}')

	return stdout.decode()

async def middleware(websocket, path):
	async for request in websocket:

		print(f'Request: {request}')

		request = json.loads(request)
		if request['type'] == 'randomization':
			result = await process_command(RANDOMIZE)
		elif request['type'] == 'simulation':
			request['id'] = uuid.uuid4().hex
			print(f"Input: {request}")

			result = await process_command(SIMULATE, request)
		else:
			raise ValueError('Unexpected JSON type.')

		print(f'Output: {result}')
		await websocket.send(result)
		print()

if __name__ == '__main__':
	print(f"Starting websocket server at {HOST}:{PORT}...")
	loop = get_event_loop()
	loop.run_until_complete(websockets.serve(middleware, HOST, PORT))
	loop.run_forever()
