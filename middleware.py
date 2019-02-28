import asyncio
import websockets

COMMAND = 'python c_program.py'


async def generate_simulation(command, data):
    process = await asyncio.create_subprocess_exec(
        *command.split(),
        stdin=asyncio.subprocess.PIPE,
        stdout=asyncio.subprocess.PIPE)
    stdout, stderr = await process.communicate(
        input=bytes(data, encoding='utf-8'))
    return stdout.decode().split('\n')[0]


async def middleware(websocket, path):
    async for message in websocket:
        print(f'Input: {message}')
        result = await generate_simulation(COMMAND, message)
        print(f'Output: {result}')
        await websocket.send(result)
        print()


asyncio.get_event_loop().run_until_complete(
    websockets.serve(middleware, 'localhost', 8888))
asyncio.get_event_loop().run_forever()