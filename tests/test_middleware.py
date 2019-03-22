import websockets
import asyncio

async def communicate(message):
    async with websockets.connect('ws://0.0.0.0:8888') as websocket:
        await websocket.send(message)
        response = await websocket.recv()
        print(f"{response}")

asyncio.get_event_loop().run_until_complete(communicate('banana'))
