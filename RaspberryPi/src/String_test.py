from time import sleep
from UUGear import *

UUGearDevice.setShowLogs(0)

device = UUGearDevice('UUGear-Arduino-1239-9170')

if device.isValid():
	print device.test_string()
else:
	print 'UUGear device is not correctly initialized.'