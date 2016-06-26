
import math

class Gcode :
	def __init__(self):
		
		self.f = 0.0	#mm/sec
		self.t = 0.0	#sec
		#current absolute values
		self.x = 0.0	#mm
		self.y = 0.0	#mm
		self.z = 0.0	#mm
		self.e = 0.0	#mm
		#previous absolute values
		self.oldx = 0.0	#mm
		self.oldy = 0.0	#mm
		self.oldz = 0.0	#mm
		self.olde = 0.0	#mm
		#travel per stepper step
		self.d = 0.0125 #mm/step
		self.dex = 0.141 #mm/step
		#stepper setps of each axis
		self.x_steps = 0
		self.y_steps = 0
		self.z_steps = 0
		self.e_steps = 0
		#stepper time between steps
		self.tx_steps = 0
		self.ty_steps = 0
		self.tz_steps = 0
		self.te_steps = 0
		#heatbed variable
		self.bed_t = 0
		#extruder heater variable
		self.ext_t = 0


	def motion_parser(self, Gline) :
		Glist = Gline.split()
		for parameter in Glist :
			if "F" in parameter :
				parameter = parameter[1:]
				self.f = float(parameter) / 60.0
			elif "X" in parameter :
				parameter = parameter[1:]
				self.x = float(parameter)
			elif "Y" in parameter :
				parameter = parameter[1:]
				self.y = float(parameter)
			elif "Z" in parameter :
				parameter = parameter[1:]
				self.z = float(parameter)
			elif "E" in parameter :
				parameter = parameter[1:]
				self.e = float(parameter)
		#print Glist
		#print "f ",self.f,"\tx ", self.x,"\ty ", self.y,"\tz " ,self.z,"\te " ,self.e

	def motion_calc (self) :
		#calculate the deltas
		dx = self.x - self.oldx
		dy = self.y - self.oldy
		dz = self.z - self.oldz
		#de = self.e - self.olde
		de = self.e
		#calculate the total motion
		h = math.pow(dx,2) + math.pow(dy,2) + math.pow(dz,2)
		h = math.sqrt(h)
		if h == 0 :
			h = de
		#calculate time of motion
		self.t = h / self.f		#sec
		#calculate feed on every axis
		fdx = dx / self.t		#mm/sec
		fdy = dy / self.t		#mm/sec
		fdz = dz / self.t		#mm/sec
		#fde = de / self.t		#mm/sec
		fde = self.f
		#calculate number of steps per axis
		self.x_steps = dx / self.d
		self.y_steps = dy / self.d 
		self.z_steps = dz / self.d
		self.e_steps = de / self.dex
		#calculate time between stepes per axis
		if self.x_steps != 0 :
			self.tx_steps = abs((self.d / fdx)*1000000)
		else :
			self.tx_steps = 0
		if self.y_steps != 0 :
			self.ty_steps = abs((self.d / fdy)*1000000)
		else :
			self.ty_steps = 0
		if self.z_steps != 0 :
			self.tz_steps = abs((self.d / fdz)*1000000/95)	#devide by 95 to make it sendable, will multiply by 95 on the arduino side
		else :
			self.tz_steps = 0
		if self.e_steps != 0 :
			self.te_steps = abs((self.dex / fde)*1000000/95)		#devide by 95 to make it sendable, will multiply by 95 on the arduino side
		else :
			self.te_steps = 0
		#ready for next calculation
		self.oldx = self.x
		self.oldy = self.y
		self.oldz = self.z
		self.olde = self.e

		#print "dx ", dx,"\tdy ", dy,"\tdz ", dz, "\tde ", de,"\th " , h,"\tt ",t
		#print "fdx ",fdx,"\tfdy",fdy,"\tfdz",fdz,"\tfde",fde
		#print "x steps ",self.x_steps ,"\ty steps ",self.y_steps,"\tz steps ",self.z_steps,"\te steps ",self.e_steps
		#print "tx ",self.tx_steps,"\tty ",self.ty_steps,"\ttz ",self.tz_steps,"\tte ",self.te_steps,"\n"

	def heatbed_parser(self,Gline) :
		Glist = Gline.split()
		for parameter in Glist :
			if "S" in parameter :
				parameter = parameter[1:]
				self.bed_t = float(parameter)
		#print Glist
		#print "s ",self.bed_t

	def extheat_parser(self,Gline) :
		Glist = Gline.split()
		for parameter in Glist :
			if "S" in parameter :
				parameter = parameter[1:]
				self.ext_t = float(parameter)
		#print Glist
		#print "ext s ",self.ext_t

	def get_X_req(self) :
		return [int(round(self.x_steps)) , int(round(self.tx_steps))]

	def get_Y_req(self) :
		return [int(round(self.y_steps)) , int(round(self.ty_steps))]

	def get_Z_req(self) :
		return [int(round(self.z_steps)) , int(round(self.tz_steps))]

	def get_E_req(self) :
		return [int(round(self.e_steps)) , int(round(self.te_steps))]

	def get_heatbed_req(self) :
		return int(round(self.bed_t))

	def get_EXTheat_req(self) :
		return int(round(self.ext_t))

	def get_exec_time(self) :
		return int(round(self.t * 1000)) #millisecond

