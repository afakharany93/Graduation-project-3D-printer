

from MidMan import *
from Gcode import *
import os



fileName = "gcode-ex"
fileSize = os.path.getsize(fileName)

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

try :
	progress = 0
	if M.is_valid():
		s1 = "I'm now printing for you " + fileName
		MidMan.stdscr.addstr(1, 9, s1)
		with open(fileName,"r") as f:
			for line in f:
				if accepted_line(line):
					s2 = "current Gcode Line: " + line
					MidMan.stdscr.addstr(2, 0, s2)
					MidMan.stdscr.refresh()
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
					M.fill_exec_time( G.get_exec_time() )

					machine = M.machine_control()
					if not(machine) :
						break
				progress = progress + len(line)
				progressPercent = (100.0*progress)/fileSize
				s15 = "Progress in terms of number of Gcode lines executed: " + str(progressPercent) + " %"
				MidMan.stdscr.addstr(15, 0, s15)
				MidMan.stdscr.refresh()
				if progressPercent == 100 :
					M.end_UI()
					print "\r\nEnd of print, please wait at least 10 minutes till all the heaters cool down before getting your print \nEnjoy :)"
				
	else :
		M.end_UI()
		print "\r\nError, won't print"	
finally :
	M.Detach_machine()		
	M.end_UI()

