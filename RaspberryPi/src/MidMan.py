from time import sleep
from UUGear import *

UUGearDevice.setShowLogs(0)

class MidMan :
	X = UUGearDevice('UUGear-Arduino-4713-9982')
	Y = UUGearDevice('UUGear-Arduino-4713-9982')
	Z = UUGearDevice('UUGear-Arduino-4713-9982')

	def __init__(self):
		self.valid = 1
		
		if MidMan.X.isValid() :
			self.valid = self.valid * self.valid
		else :
			print "X axis isn't correctly initialized"
			self.valid = 0

		if MidMan.Y.isValid() :
			self.valid = self.valid * self.valid
		else :
			print "Y axis isn't correctly initialized"
			self.valid = 0

		if MidMan.Z.isValid() :
			self.valid = self.valid * self.valid
		else :
			print "Z axis isn't correctly initialized"
			self.valid = 0

	def is_valid (self) :
		return self.valid