import websockets
import asyncio
import pytest


def connect(func):
	async def wrapper():
		async with websockets.connect('ws://0.0.0.0:8888') as websocket:
			await func(websocket)
	return wrapper


@pytest.mark.asyncio
@connect
async def test_bad_request(websocket):
	"""
	Middleware should only take in a JSON object.
	"""
	await websocket.send('banana')
	result = await websocket.recv()
	assert 'InvalidJSON' in result

