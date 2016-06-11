from Gcode import *

def accepted_line(txt) :
	if txt.find("G0") != -1 :
		return True
	elif txt.find("G1") != -1 :
		return True
	elif txt.find("M190") != -1 :
		return True
	elif txt.find("M104") != -1:
		return True
	else :
		return False		

G = Gcode()
with open("gcode-ex","r") as f:
	for line in f:
		if accepted_line(line):
			if line.find("G0") != -1 :
				G.motion_parser(line)
				G.motion_calc()
			elif line.find("G1") != -1 :
				G.motion_parser(line)
				G.motion_calc()
			elif line.find("M190") != -1 :
				G.heatbed_parser(line)
			elif line.find("M104") != -1:
				G.extheat_parser(line)

			print G.get_X_req()
			print G.get_Y_req()
			print G.get_Z_req()
			print G.get_E_req()
			print G.get_heatbed_req()
			print G.get_EXTheat_req()
			

