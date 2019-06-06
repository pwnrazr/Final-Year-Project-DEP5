#!/usr/bin/env python
#Smart Mailbox

#First written on 8th of December 2018
#Updated on 6th of January 2019

import smtplib

try:
    import RPi.GPIO as GPIO
	
except RuntimeError:
    print("Error importing RPi.GPIO!  This is probably because you need superuser privileges.  You can achieve this by using 'sudo' to run your script")	#just in case
	
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.base import MIMEBase
from email import encoders
from picamera import PiCamera
import time

SwitchChan = 4 	#The channel that reed switch is connected to
TRIG = 17  		#Ultrasonic sensor
ECHO = 27		#Ultrasonic sensor
LED = 22		#Pin for LED 'flash'

#GPIO Setup
GPIO.setmode(GPIO.BCM)
GPIO.setup(SwitchChan, GPIO.IN) #set GPIO as input
GPIO.setup(SwitchChan, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)	#Enable internal pull-up resistor
GPIO.setup(LED, GPIO.OUT)	#set GPIO as output
GPIO.setup(TRIG, GPIO.OUT)
GPIO.setup(ECHO, GPIO.IN)

GPIO.add_event_detect(SwitchChan, GPIO.RISING, bouncetime=5000) #Rising edge detection for reed switch with debounce time of 3500ms
GPIO.output(TRIG, False)	#set to LOW or 0 just to be sure
GPIO.output(LED, False)		#set to LOW or 0 just to be sure

try:
	while True:

		time.sleep(0.01) #To reduce power and cpu usage while polling for state change 

		if GPIO.event_detected(SwitchChan):
			print "Mail insertion detected!"
			time.sleep(2) 	#Time to wait in seconds before continuing after mail insertion is detected

			#Begin distance measurement
			GPIO.output(TRIG, False)
			print "Getting distance from ultrasonic sensor"
			time.sleep(2)

			GPIO.output(TRIG, True)
			time.sleep(0.00001)
			GPIO.output(TRIG, False)

			while GPIO.input(ECHO)==0:
  				pulse_start = time.time()

			while GPIO.input(ECHO)==1:
 				pulse_end = time.time()

			pulse_duration = pulse_end - pulse_start

			distance = pulse_duration * 17150

			distance = round(distance, 2)

			print "Distance:",distance,"cm" #debug
			#End distance measurement

			#take picture
			print "Taking picture"; #debug
			GPIO.output(LED, True) #Turn on 'flash'
			camera = PiCamera() #Starts camera
			time.sleep(0.5) #Probably helps with camera's auto exposure
			camera.capture('/home/pi/Desktop/testimage.jpg') #Takes one picture
			camera.close() #Close camera to save battery
			GPIO.output(LED, False) #Turn off 'flash'
			print "Picture taken"; #debug
			#take picture end
			
			#Email function
			fromaddr = "AmirzFYP@gmail.com" #Made a gmail account just for this
			toaddr = "amirz98@hotmail.com"  #Will send email to my account

			msg = MIMEMultipart()
			msg['From'] = fromaddr
			msg['To'] = toaddr
			msg['Subject'] = "You got mail!" #Subject of email

			#body = "You just received mail! Distance is %.2fcm {Placeholder for until we have a more proper box}" % distance #Body of email

			#Change body of email based on how full the mailbox is
			if distance > 11:

				body = "You just received mail!"
				print "The mailbox is empty/small amount of mail";

			elif distance < 11 and distance > 7:

				body = "You just received mail! Your mailbox is half full"
				print "The mailbox is half full";

			elif distance <= 7:

				body = "You just received mail! WARNING! Your mailbox is full/almost full"
				print "The mailbox is full/almost full";

			msg.attach(MIMEText(body, 'plain'))

			filename = "MailPic.jpg" #name of the file displayed in the email
			attachment = open("/home/pi/Desktop/testimage.jpg", "rb") #Location of image to be sent

			print "Attached image to email"; #debug	

			part = MIMEBase('application', 'octet-stream')
			part.set_payload((attachment).read())
			encoders.encode_base64(part)
			part.add_header('Content-Disposition', "attachment; filename= %s" % filename)
			msg.attach(part)

			server = smtplib.SMTP('smtp.gmail.com', 587)
			server.starttls()
			server.login(fromaddr, "razr12345") #Source email password - not my real account so no worries about password
			text = msg.as_string()
			print "Attempting to send email"; #debug
			server.sendmail(fromaddr, toaddr, text)
			print "Email has been sent successfully"; #debug
			server.quit()
			#email function end

except KeyboardInterrupt:
	GPIO.cleanup() #Cleanup GPIO stuff
	print "Code stopped by keyboard interrupt";