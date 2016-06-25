from time import sleep
from UUGear import *

UUGearDevice.setShowLogs(0)

device = UUGearDevice('UUGear-Arduino-5658-7598')

if device.isValid():
		response1 = device.stepper_time_bet_steps(1000)
		print response1
		status = device.stepper_status()
		print status

		response1 = device.stepper_move(5*800)
		print response1
		status = device.stepper_status()
		print status

		for i in range(100) :
			status = device.stepper_status()
			print status
			sleep(0.1)


		device.detach()
		device.stopDaemon()
else:
	print 'UUGear device is not correctly initialized.'