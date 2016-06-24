from time import sleep
from UUGear import *

UUGearDevice.setShowLogs(0)

device = UUGearDevice('UUGear-Arduino-3167-3008')

if device.isValid():
	try:
		r = device.heatbed_set_temp(60);
		print r
		for i in range(200):
			status = device.heatbed_status()
			print status

			sleep(0.2)

		#r=device.ext_stepper_time_bet_steps(40000)
		#print r
		#r=device.ext_stepper_move(4000)
		#print r
		#for i in range(200):
		#	status = device.ext_heat_status()
		#	print status

		#	sleep(0.2)
		r = device.heatbed_set_temp(1);
		print r
		status = device.heatbed_status()
		print status
	finally:
		device.detach()
		device.stopDaemon()
else:
	print 'UUGear device is not correctly initialized.'
