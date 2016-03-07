from time import sleep
from UUGear import *

UUGearDevice.setShowLogs(0)

device = UUGearDevice('UUGear-Arduino-1239-9170')

if device.isValid():
	response1 = device.stepper_time_bet_steps(400)
	print response1
	sleep(3)
	response = device.stepper_move(1000)
	print response
	
	sleep(3)
	response = device.stepper_go_home()
	print response

	sleep(10)
	response = device.stepper_stop()
	print response

	sleep(3)
	response = device.stepper_resume()
	print response

	sleep(10)
	response = device.stepper_stop()
	print response

	device.detach()
	device.stopDaemon()
else:
	print 'UUGear device is not correctly initialized.'