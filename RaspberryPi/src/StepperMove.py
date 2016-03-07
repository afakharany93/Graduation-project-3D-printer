from time import sleep
from UUGear import *

UUGearDevice.setShowLogs(0)

device = UUGearDevice('UUGear-Arduino-1239-9170')
device1 = UUGearDevice('UUGear-Arduino-4713-9982')

if device.isValid():
	if device1.isValid():
		response1 = device.stepper_time_bet_steps(400)
		print response1

		response1 = device1.stepper_time_bet_steps(400)
		print response1+1
		
		sleep(3)
		response = device.stepper_move(1000)
		print response

		response = device1.stepper_move(1000)
		print response+1
		
		sleep(3)
		response = device.stepper_go_home()
		print response

		response = device1.stepper_go_home()
		print response+1

		sleep(10)
		response = device.stepper_stop()
		print response

		response = device1.stepper_stop()
		print response+1

		sleep(3)
		response = device.stepper_resume()
		print response

		response = device1.stepper_resume()
		print response+1

		sleep(10)
		response = device.stepper_stop()
		print response

		response = device1.stepper_stop()
		print response+1

		device1.detach()
		device.detach()
		device.stopDaemon()
	else :
		print "device1 not init"
else:
	print 'UUGear device is not correctly initialized.'