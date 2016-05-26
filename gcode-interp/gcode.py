class Gcode :
	def __init__(self):
		self.f = 0.0	#mm/sec
		self.h = 0.0	#mm , length moved in x,y and z combined
		self.x = 0.0	#mm
		self.y = 0.0	#mm
		self.z = 0.0	#mm
		self.e = 0.0	#mm
		
		self.oldx = 0.0	#mm
		self.oldy = 0.0	#mm
		self.oldz = 0.0	#mm
		self.olde = 0.0	#mm
		
		self.dx = 0.0 #mm
		self.dy = 0.0 #mm
		self.dz = 0.0 #mm
		self.de = 0.0 #mm


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
		print Glist
		print self.f, self.x, self.y, self.z, self.e

	def motion_calc (self) :
		self.dx = self.x - self.oldx
		self.dy = self.y - self.oldy
		self.dz = self.z - self.oldz
		self.de = self.e - self.olde



		

G = Gcode()
with open("gcode-ex","r") as f:
	for line in f:
		if line.find("G0") != -1 :
			G.motion_parser(line)
		elif line.find("G1") != -1 :
			G.motion_parser(line)
			

