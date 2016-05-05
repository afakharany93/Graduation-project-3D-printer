from time import sleep
from UUGear import *

UUGearDevice.setShowLogs(0)

device = UUGearDevice('UUGear-Arduino-1239-9170')

if device.isValid():
		r = device.heatbed_set_temp(60);
		print r
		for i in range(50):
			status = device.heatbed_status()
			print status

			sleep(0.1)

		r = device.heatbed_set_temp(0);
		print r
		status = device.heatbed_status()
		print status
		device.detach()
		device.stopDaemon()
else:
	print 'UUGear device is not correctly initialized.'