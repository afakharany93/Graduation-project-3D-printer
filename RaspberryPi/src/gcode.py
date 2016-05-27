from MidMan import *
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

M = MidMan()
G = Gcode()
if M.is_valid():
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

				M.fill_Xdata( G.get_X_req() )
				M.fill_Ydata( G.get_Y_req() )
				M.fill_Zdata( G.get_Z_req() )
				M.fill_Edata( G.get_E_req() )
				M.fill_heatbed_data( G.get_heatbed_req() )
				M.fill_ext_heat_data( G.get_EXTheat_req() )
else :
	print "Error, won't print"			

