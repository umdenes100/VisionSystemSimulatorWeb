import subprocess
import decorator
import json
import os


COMMAND = './randomize'
WORKING_DIR = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'server', 'randomization')

def randomize(func):
	def wrapper():

		process = subprocess.Popen(COMMAND, 
								   cwd=WORKING_DIR,
								   stdin=subprocess.PIPE,
	                               stdout=subprocess.PIPE,
	                               stderr=subprocess.PIPE)
		stdout, stderr = process.communicate()

		randomization = json.loads(stdout.decode())
		func(randomization)

	return wrapper

@randomize
def test_format(randomization):
	assert isinstance(randomization, dict)
	assert 'type' in randomization
	assert 'osv' in randomization
	assert 'obstacles' in randomization
	assert 'destination' in randomization
