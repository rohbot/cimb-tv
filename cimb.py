import serial
import os
import sys
import random
import time
import subprocess
import threading
import redis
import random
import logging

from os import listdir
from os.path import isfile, join

#logging.basicConfig(filename='/home/pi/bc.log',level=logging.DEBUG)
logging.basicConfig(filename='/home/pi/cimb.log',level=logging.DEBUG)
logging.info(time.ctime())
logging.info("Initialsing.... waiting for stuff") 

#MOVIE_DIR = '/home/pi/movies/'

#files = ['tiger.m4v', '1-rangtan.mp4', '2-PowerDown2PowerAll.mp4', 'founders-valley.mp4']

#time.sleep(10)
logging.info( "Starting...")

strPort1 = '/dev/ttyACM0'
#strPort1 = '/dev/ttyACM1'

ser = serial.Serial(strPort1, 115200)

# def on_connect(client, userdata, flags, rc):
#     logging.info(("Connected with result code "+str(rc))
#     logging.info( "led on"
#     client.publish(LED_TOPIC, 'ON')


# LED_TOPIC = 'lab/light2/switch'

# LIGHT_ON_CMD = "mqtt pub -h '192.168.0.30' -t 'lab/light2/switch' -m 'ON'"
# LIGHT_OFF_CMD = "mqtt pub -h '192.168.0.30' -t 'lab/light2/switch' -m 'OFF'"

r = redis.Redis()

HDMI_OFF_CMD = "vcgencmd display_power 0"
HDMI_ON_CMD = "vcgencmd display_power 1"

OMX_START_CMD = 'omxplayer -b -r -o local '
OMX_STOP_CMD = 'echo -n q > /tmp/cmd &'
OMX_PAUSE_CMD = 'echo -n p > /tmp/cmd &'
OMX_KILL_CMD = 'killall omxplayer.bin'

MOVIE_DIR = '/home/pi/movies/'

BLACK_SCREEN_CMD = "sudo fbi -T 2 -a --noverbose  /home/pi/cimb/black.jpg"
LOGO_SCREEN_CMD = "sudo fbi -T 2 -a --noverbose  /home/pi/cimb/cimb.jpg"


try:
	os.mkfifo("/tmp/cmd")
	logging.info("Temp file created!")

except OSError as e:
	logging.info("Temp file already exists")
	# 17 means the file already exists.
	if e.errno != 17:
		raise


paused = False

omxplaying = False
screenOn = False

lastPaused = time.time()
lastPlay = time.time() 
#os.system(LIGHT_ON_CMD)
#os.system(HDMI_OFF_CMD)			
#os.system(OMX_KILL_CMD)
os.system(BLACK_SCREEN_CMD)


def checkVideoPlaying():
	global screenOn
	try:
		a = subprocess.check_output("pgrep omxplayer.bin", shell=True)
 		
 		return True
	except:
		logging.info( "video not playing")
		
		
		return False


def playVideo(videoFileName):
	os.system(OMX_START_CMD + videoFileName + ' < /tmp/cmd &')
	logging.info("Playing:" + videoFileName)

logging.info( "Ready")
r.set('bcomx:start', time.ctime())
checkVideoPlaying()

last_on = time.time()


while True:

	#client.loop()
	line = ser.readline().strip()
	try:
		tokens = line.split(",")
		print tokens
		if tokens[0] == 'x':
			val = int(tokens[1])
			if val == 1:
				os.system(LOGO_SCREEN_CMD)
			else:
				os.system(BLACK_SCREEN_CMD)
		#else:


		# if len(line) < 1 or len(tokens) < 1 or line[0] in ['s']:
		# 	logging.info(line)
		# else:
		# 	tokens = line.split(",")
		# 	#logging.info( tokens )

		# 	val = float(tokens[1])
		# 	logging.info(val)
		# 	if val > 100 and time.time() - last_on > 10:
		# 		last_on = time.time()
		# 		#omxplaying = checkVideoPlaying()
		# 		if not omxplaying:
		# 			#omxplaying = True
		# 			logging.info( "HDMI on")
		# 			#turn on HDMI
		# 			os.system(HDMI_ON_CMD)
		# 			screenOn = True
		# 			#time.sleep(1)
		# 			#logging.info( "Player movie")
		# 			#os.system(LIGHT_OFF_CMD)
		# 			file  = MOVIE_DIR + files[fi] 
		# 			fi += 1
		# 			if fi >= len(files):
		# 				fi = 0
					
		# 			thread = threading.Thread(target=playVideo, args=(file,) )
		# 			thread.start()
		# 			omxplaying = True
					
		# 			os.system("echo . > /tmp/cmd &")  # Start signal for OMXplayer
					
		# 			paused = False
				
		# 		if paused:
		# 			os.system(HDMI_ON_CMD)
		# 			os.system(OMX_PAUSE_CMD)
		# 			paused = False
		# 			logging.info("Continue video")	


		# 	if val < 5 and omxplaying:
		# 		if not paused:
		# 			paused = True

		# 			os.system(OMX_PAUSE_CMD)
					
		# 			#os.system(LIGHT_ON_CMD)
		# 			os.system(HDMI_OFF_CMD)
		# 			screenOn = False
		# 			logging.info( "pausing video")
		# 			lastPaused = time.time()

		# 	if val < 4.9:
		# 	  if omxplaying:
		# 	  	os.system(OMX_STOP_CMD)
		# 		logging.info( "stoping video" + file )
		# 	  	omxplaying = checkVideoPlaying() 	
			
		# 	#publish cap value to redis for future logging
		# 	r.publish("cap" , val)
					

		# if omxplaying and time.time() - last_on > 5:
		# 	omxplaying = checkVideoPlaying()			
		
		# 	if not omxplaying:
		# 		logging.info("Turning off HDMI")
		# 		os.system(HDMI_OFF_CMD)			
		# 		screenOn = False


	except KeyboardInterrupt:
		raise
	except:
		#raise
		e = sys.exc_info()

		logging.info( "something messed up")
		logging.error(e)
		
