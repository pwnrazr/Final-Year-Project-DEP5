#!/usr/bin/env python

import time

try:
	while True:
		print "waka waka";
		time.sleep(1)
except KeyboardInterrupt:
	print "Interrupted whao!";