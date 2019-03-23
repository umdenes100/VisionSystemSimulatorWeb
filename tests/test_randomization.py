import subprocess
import json
import os


COMMAND = './randomize'
WORKING_DIR = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'server', 'randomization')

def get_randomization():
	process = subprocess.Popen(COMMAND, 
							   cwd=WORKING_DIR,
							   stdin=subprocess.PIPE,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE)
	stdout, stderr = process.communicate()

	randomization = json.loads(stdout.decode())

	return randomization

def test_randomization_format():

	randomization = get_randomization()

	assert isinstance(randomization, dict)
	assert 'type' in randomization
	assert 'osv' in randomization
	assert 'obstacles' in randomization
	assert 'destination' in randomization
