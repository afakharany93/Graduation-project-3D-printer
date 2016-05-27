from time import sleep
from UUGear import *

UUGearDevice.setShowLogs(0)

class MidMan :
	X = UUGearDevice('UUGear-Arduino-4713-9982')
	Y = UUGearDevice('UUGear-Arduino-4713-9982')
	Z = UUGearDevice('UUGear-Arduino-4713-9982')

	def __init__(self):
		self.valid = 1

		self.Xlist = []
		self.Ylist = []
		self.Zlist = []
		self.Elist = []
		self.heatbed_t = 0
		self.ext_heat = 0
		#check validity
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

	def fill_Xdata(self, lst) :
		self.Xlist = lst

	def fill_Ydata(self, lst) :
		self.Ylist = lst

	def fill_Zdata(self, lst) :
		self.Zlist = lst

	def fill_Edata(self, lst) :
		self.Elist = lst

	def fill_heatbed_data(self, var) :
		self.heatbed_t = var

	def fill_ext_heat_data(self, var) :
		self.ext_heat = var
