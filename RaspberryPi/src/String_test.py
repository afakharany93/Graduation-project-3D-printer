from time import sleep
from UUGear import *

UUGearDevice.setShowLogs(0)

device = UUGearDevice('UUGear-Arduino-3167-3008')

if device.isValid():
	print device.stepper_status()

	device.detach()
	device.stopDaemon()
else:
	print 'UUGear device is not correctly initialized.'