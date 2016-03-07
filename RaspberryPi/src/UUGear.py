 #
 # This is part of a graduation project solution aims to build a 3d printer,
 # made by students at the AASTMT sheraton, Mechatronics department, 2016
 # https://github.com/afakharany93/Graduation-project-3D-printer
 #
 # Used UUgear for communication
 #
 # Authors:
 # Ahmed El Fakharany - afakharany93@gmail.com
 #
 #######################################################################
 
from ctypes import CDLL, Structure, POINTER, byref, c_char, c_int, c_float, c_char_p , c_short , c_ushort

UUGEAR_ID_MAX_LENGTH = 1024

uugearlib = CDLL('./libUUGear.so')

class UUGearDeviceProfile(Structure):
    _fields_ = [
    	('id', (c_char * UUGEAR_ID_MAX_LENGTH)), 
    	('clientId', c_int), 
    	('fd', c_int), 
    	('in', c_int), 
    	('out', c_int)
    ]

uugearlib.setShowLogs.restype = None
uugearlib.setShowLogs.argtypes = [c_int]

uugearlib.attachUUGearDevice.restype = UUGearDeviceProfile
uugearlib.attachUUGearDevice.argtypes = [c_char_p]

uugearlib.detachUUGearDevice.restype = None
uugearlib.detachUUGearDevice.argtypes = [POINTER(UUGearDeviceProfile)]

uugearlib.stepper_move.restype = c_int
uugearlib.stepper_move.argtypes = [POINTER(UUGearDeviceProfile), c_short]

uugearlib.stepper_time_bet_steps.restype = c_int
uugearlib.stepper_time_bet_steps.argtypes = [POINTER(UUGearDeviceProfile), c_ushort]

uugearlib.stepper_go_home.restype = c_int
uugearlib.stepper_go_home.argtypes = [POINTER(UUGearDeviceProfile)]

uugearlib.stepper_stop.restype = c_int
uugearlib.stepper_stop.argtypes = [POINTER(UUGearDeviceProfile)]



class UUGearDevice(object):
	
	devProfile = None

	@staticmethod
	def setShowLogs(show):
		uugearlib.setShowLogs(show)
	
	def __init__(self, id):
		uugearlib.setupUUGear()
		self.devProfile = uugearlib.attachUUGearDevice(id)
		
	def isValid(self):
		return self.devProfile != None and self.devProfile.fd != -1
	
	def detach(self):
		if self.isValid():
			uugearlib.detachUUGearDevice(byref(self.devProfile))
			self.devProfile = None	
		
	def stopDaemon(self):
		uugearlib.cleanupUUGear()
	

	def stepper_move(self, steps):
		if self.isValid():
			return uugearlib.stepper_move(byref(self.devProfile), steps)
		else :
			return -1

	def stepper_time_bet_steps(self, time_us):
		if self.isValid():
			return uugearlib.stepper_time_bet_steps(byref(self.devProfile), time_us)
		else :
			return -1

	def stepper_go_home (self) :
		if self.isValid():
			return uugearlib.stepper_go_home(byref(self.devProfile))
		else :
			return -1

	def stepper_stop (self) :
		if self.isValid():
			return uugearlib.stepper_stop(byref(self.devProfile))
		else :
			return -1
