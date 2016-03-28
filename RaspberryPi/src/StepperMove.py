from time import sleep
from UUGear import *

UUGearDevice.setShowLogs(0)

device = UUGearDevice('MCU_Arduino-3921-4545')

if device.isValid():
		response1 = device.stepper_time_bet_steps(400)
		print response1
		status = device.stepper_status()
		print status

		response1 = device.stepper_go_home()
		print response1
		status = device.stepper_status()
		print status


		device.detach()
		device.stopDaemon()
else:
	print 'UUGear device is not correctly initialized.'