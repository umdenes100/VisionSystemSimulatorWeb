from asyncio import create_subprocess_exec, subprocess, get_event_loop
import websockets
import json

HOST = '0.0.0.0'
PORT = 8888

RANDOMIZE = './randomization/randomize'
SIMULATE = './simulator/simulate'

async def process_command(command, data=None):
	process = await create_subprocess_exec(*command.split(), 
										   stdin=subprocess.PIPE, 
										   stdout=subprocess.PIPE)
	if data is None:
		stdout, stderr = await process.communicate()
	else:
		stdout, stderr = await process.communicate(input=bytes(data, encoding='utf-8'))

	return stdout.decode()

async def middleware(websocket, path):
	async for message in websocket:

		print(f'Message: {message}')

		message = json.loads(message)
		if message['type'] == 'randomization':
			result = await process_command(RANDOMIZE)
		elif message['type'] == 'simulation':
			print(f"Input: {message['content']}")

			result = await process_command(SIMULATE, message['content'])
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
