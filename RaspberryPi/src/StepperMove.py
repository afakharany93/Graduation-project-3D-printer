from time import sleep
from UUGear import *

UUGearDevice.setShowLogs(0)

device = UUGearDevice('UUGear-Arduino-1239-9170')

if device.isValid():
	response = device.stepper_move(2000)
	print response
	
	device.detach()
	device.stopDaemon()
else:
	print 'UUGear device is not correctly initialized.'