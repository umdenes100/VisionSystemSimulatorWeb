from asyncio import create_subprocess_exec, subprocess, get_event_loop
import websockets

COMMAND = './randomization/randomize'


async def generate_simulation(command, data):
	process = await create_subprocess_exec(*command.split(), stdin=subprocess.PIPE, stdout=subprocess.PIPE)
	# stdout, stderr = await process.communicate(input=bytes(data, encoding='utf-8'))
	stdout, stderr = await process.communicate()
	return stdout.decode()


async def middleware(websocket, path):
	async for message in websocket:
		print(f'Input: {message}')
		result = await generate_simulation(COMMAND, message)
		print(f'Output: {result}')
		await websocket.send(result)
		print()

if __name__ == '__main__':
	print("Starting websocket server...")
	loop = get_event_loop()
	loop.run_until_complete(websockets.serve(middleware, '0.0.0.0', 8888))
	loop.run_forever()
