from time import sleep
from UUGear import *

UUGearDevice.setShowLogs(0)

device = UUGearDevice('UUGear-Arduino-3167-3008')

if device.isValid():
		for i in range(5):
			status = device.temperature_status()
			print status

			sleep(0.5)

		device.detach()
		device.stopDaemon()
else:
	print 'UUGear device is not correctly initialized.'