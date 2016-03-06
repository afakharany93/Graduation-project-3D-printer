from time import sleep
from UUGear import *

UUGearDevice.setShowLogs(0)

device = UUGearDevice('UUGear-Arduino-1239-9170')

if device.isValid():
	response1 = device.stepper_time_bet_steps(1000)
	print response1
	sleep(2.5)
	response = device.stepper_move(2000)
	print response
	
	device.detach()
	device.stopDaemon()
else:
	print 'UUGear device is not correctly initialized.'