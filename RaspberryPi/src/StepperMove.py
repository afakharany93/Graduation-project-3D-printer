from time import sleep
from UUGear import *

UUGearDevice.setShowLogs(0)

device = UUGearDevice('UUGear-Arduino-7309-1013')

if device.isValid():
	try:
		res=device.heatbed_set_temp(100)
		while (device.heatbed_status() < 0.85 *2.5*100)

		response1 = device.ext_stepper_time_bet_steps(1000/95)
		print response1
		status = device.ext_stepper_status()
		print status

		response1 = device.ext_stepper_move(5*800)
		print response1
		status = device.ext_stepper_status()
		print status

		for i in range(100) :
			status = device.ext_stepper_status()
			print status
			sleep(0.1)

	finally:
		device.detach()
		device.stopDaemon()
else:
	print 'UUGear device is not correctly initialized.'