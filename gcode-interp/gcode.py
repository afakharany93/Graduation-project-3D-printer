from Gcode import *
		

G = Gcode()
with open("gcode-ex","r") as f:
	for line in f:
		if line.find("G0") != -1 :
			G.motion_parser(line)
			G.motion_calc()
		elif line.find("G1") != -1 :
			G.motion_parser(line)
			G.motion_calc()
			

