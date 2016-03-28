from time import sleep
from UUGear import *

UUGearDevice.setShowLogs(0)

device = UUGearDevice('MCU-Arduino-3921-4545')

if device.isValid():
	print device.stepper_status()

	device.detach()
	device.stopDaemon()
else:
	print 'UUGear device is not correctly initialized.'