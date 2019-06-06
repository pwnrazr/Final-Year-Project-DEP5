try:
    import RPi.GPIO as GPIO
	
except RuntimeError:
    print("Error importing RPi.GPIO!  This is probably because you need superuser privileges.  You can achieve this by using 'sudo' to run your script")
	
import time
	
GPIO.setmode(GPIO.BCM)
GPIO.setup(4, GPIO.IN)
GPIO.setup(4, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.add_event_detect(4, GPIO.RISING, bouncetime=2500)

try:
	while True:
		if GPIO.event_detected(4):
			print "its HIGH! whao";
			#time.sleep(5)
except KeyboardInterrupt:
	print "Interrupted whao!";