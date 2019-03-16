import sys
import time
import random

delay = random.uniform(0, 2)

time.sleep(delay)

sys.stdout.write('Delay {:1.5f} - '.format(delay) + str(sys.stdin.readlines()))
