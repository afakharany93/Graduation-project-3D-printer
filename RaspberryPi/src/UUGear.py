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
 
from ctypes import CDLL, Structure, POINTER, byref, c_char, c_int, c_float, c_char_p , c_short , c_ushort , c_char_p

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

uugearlib.stepper_resume.restype = c_int
uugearlib.stepper_resume.argtypes = [POINTER(UUGearDeviceProfile)]

uugearlib.stepper_status.restype = c_char_p
uugearlib.stepper_status.argtypes = [POINTER(UUGearDeviceProfile)]

uugearlib.temperature_status.restype = c_char_p
uugearlib.temperature_status.argtypes = [POINTER(UUGearDeviceProfile)]

uugearlib.heatbed_status.restype = c_char_p
uugearlib.heatbed_status.argtypes = [POINTER(UUGearDeviceProfile)]

uugearlib.heatbed_set_temp.restype = c_int
uugearlib.heatbed_set_temp.argtypes = [POINTER(UUGearDeviceProfile), c_int]

uugearlib.ext_heat_status.restype = c_char_p
uugearlib.ext_heat_status.argtypes = [POINTER(UUGearDeviceProfile)]

uugearlib.ext_heat_set_temp.restype = c_int
uugearlib.ext_heat_set_temp.argtypes = [POINTER(UUGearDeviceProfile), c_int]



class UUGearDevice(object):
	
	devProfile = None

	@staticmethod
	def setShowLogs(show):
		uugearlib.setShowLogs(show)
	
	def __init__(self, id):
		uugearlib.setupUUGear()
		self.devProfile = uugearlib.attachUUGearDevice(id)
		self.stepper_status_dict = {1 : 'Stepper is moving', 2 : 'Stepper has ended motion', 
									3 : 'Stepper Stopped due to SW', 4 : 'Stepper resuming after stopping by SW' ,
									5 : 'Stepper is flowing'}

		self.endstop_status_dict = {0 : 'No endstops pressed', 1 : 'Home endstop pressed', 2 : 'Away endstop pressed'}
		
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

	def stepper_resume (self) :
		if self.isValid():
			return uugearlib.stepper_resume(byref(self.devProfile))
		else :
			return -1

	def stepper_status (self)	:
		if self.isValid():
			buf =  uugearlib.stepper_status(byref(self.devProfile))
			if buf.find("Status 1") != -1 :
				buf = buf.replace("1", self.stepper_status_dict[1], 1)
			elif buf.find("Status 2") != -1 :
				buf = buf.replace("2", self.stepper_status_dict[2], 1)
			elif buf.find("Status 3") != -1 :
				buf = buf.replace("3", self.stepper_status_dict[3], 1)
			elif buf.find("Status 4") != -1 :
				buf = buf.replace("4", self.stepper_status_dict[4], 1)
			elif buf.find("Status 5") != -1 :
				buf = buf.replace("5", self.stepper_status_dict[5], 1)

			if buf.find("endstops 0") != -1 :
				buf = buf.replace("endstops 0", self.endstop_status_dict[0], 1)
			elif buf.find("endstops 1") != -1 :
				buf = buf.replace("endstops 1", self.endstop_status_dict[1], 1)
			elif buf.find("endstops 2") != -1 :
				buf = buf.replace("endstops 2", self.endstop_status_dict[2], 1)

			return buf
		else :
			return -1

	def temperature_status (self)	:
		if self.isValid():
			buf =  uugearlib.temperature_status(byref(self.devProfile))
			return buf
		else :
			return -1

	def heatbed_status (self)	:
		if self.isValid():
			buf =  uugearlib.heatbed_status(byref(self.devProfile))
			return buf
		else :
			return -1

	def heatbed_set_temp(self, temp):
		if self.isValid():
			return uugearlib.heatbed_set_temp(byref(self.devProfile), temp)
		else :
			return -1

	def ext_heat_status (self)	:
		if self.isValid():
			buf =  uugearlib.ext_heat_status(byref(self.devProfile))
			return buf
		else :
			return -1

	def ext_heat_set_temp(self, temp):
		if self.isValid():
			return uugearlib.ext_heat_set_temp(byref(self.devProfile), temp)
		else :
			return -1
