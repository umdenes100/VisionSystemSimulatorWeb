import sys
import time
import random

delay = random.uniform(0, 2)

time.sleep(delay)

for line in sys.stdin:
	sys.stdout.write('Delay {:1.5f} - '.format(delay) + line)
