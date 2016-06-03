from time import sleep
from UUGear import *
import curses

UUGearDevice.setShowLogs(0)

class MidMan :
	X = UUGearDevice('UUGear-Arduino-1239-9170')	#X axis stepper and extruder heater
	Y = UUGearDevice('UUGear-Arduino-3167-3008')	#Y axis stepper and heatbed
	Z = UUGearDevice('UUGear-Arduino-5658-7598')	#Z axis stepper and extruder stepper
	stdscr = curses.initscr()
	
	def __init__(self):
		MidMan.stdscr.keypad(1)
		curses.noecho()
		
		self.valid = 1

		self.Xlist = [0,0]
		self.Ylist = [0,0]
		self.Zlist = [0,0]
		self.Elist = [0,0]
		self.heatbed_t = 0
		self.old_heatbed_t = 0
		self.ext_heat = 0
		self.old_ext_heat = 0
		self.exec_time = 0		#execution time in milliseconds

		self.Xremain_steps = 0
		self.Yremain_steps = 0
		self.Zremain_steps = 0
		self.Eremain_steps = 0
		self.Xt_bet_steps_stat = 0
		self.Yt_bet_steps_stat = 0 
		self.Zt_bet_steps_stat = 0
		self.Et_bet_steps_stat = 0
		self.heatbed_t_stat = 0
		self.ext_t_stat = 0
		#check validity
		if MidMan.X.isValid() :
			self.valid = self.valid * self.valid
		else :
			self.end_UI()
			print "\r\nX axis controller isn't correctly initialized. Hint : check UUgear ID"
			self.valid = 0

		if MidMan.Y.isValid() :
			self.valid = self.valid * self.valid
		else :
			self.end_UI()
			print "\r\nY axis controller isn't correctly initialized. Hint : check UUgear ID"
			self.valid = 0

		if MidMan.Z.isValid() :
			self.valid = self.valid * self.valid
		else :
			self.end_UI()
			print "\r\nZ axis controller isn't correctly initialized. Hint : check UUgear ID"
			self.valid = 0

	def is_valid (self) :
		return self.valid

	def end_UI (self) :
		MidMan.stdscr.keypad(0)
		curses.echo()
		curses.nocbreak()
		curses.endwin()                 # Terminate curses

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

	def fill_exec_time(self, var) :
		self.exec_time = var

	def send_Xdata(self) :
		if self.Xlist[0] != 0 :
			#send time between steps
			res = MidMan.X.stepper_time_bet_steps(self.Xlist[1])
			#if no valid response try again
			if res == -1 or res != 47 :
				res = MidMan.X.stepper_time_bet_steps(self.Xlist[1])
				#if again no valid response, flag an error
				if res == -1 or res != 47 :
					self.end_UI()
					print "\r\nError sending stepper_time_bet_steps. Hint: check if stepper module is properly defined in X axis controller"
					return False
			#if time between steps is sent properly then proceed to send the number of steps
			res1 = MidMan.X.stepper_move(self.Xlist[0])
			#if no valid response try again
			if res1 == -1 or res1 != 47 :
				res1 = MidMan.X.stepper_move(self.Xlist[0])
				#if again no valid response, flag an error
				if res1 == -1 or res1 != 47 :
					self.end_UI()
					print "\r\nError sending stepper_move. Hint: check if stepper module is properly defined in X axis controller"
					return False
		return True

	def send_Ydata(self) :
		if self.Ylist[0] != 0 :
			#send time between steps
			res = MidMan.Y.stepper_time_bet_steps(self.Ylist[1])
			#if no valid response try again
			if res == -1 or res != 47 :
				res = MidMan.Y.stepper_time_bet_steps(self.Ylist[1])
				#if again no valid response, flag an error
				if res == -1 or res != 47 :
					self.end_UI()
					print "\r\nError sending stepper_time_bet_steps. Hint: check if stepper module is properly defined in Y axis controller"
					return False
			#if time between steps is sent properly then proceed to send the number of steps
			res1 = MidMan.Y.stepper_move(self.Ylist[0])
			#if no valid response try again
			if res1 == -1 or res1 != 47 :
				res1 = MidMan.Y.stepper_move(self.Ylist[0])
				#if again no valid response, flag an error
				if res1 == -1 or res1 != 47 :
					self.end_UI()
					print "\r\nError sending stepper_move. Hint: check if stepper module is properly defined in Y axis controller"
					return False
		return True

	def send_Zdata(self) :
		if self.Zlist[0] != 0 :
			#send time between steps
			res = MidMan.Z.stepper_time_bet_steps(self.Zlist[1])
			#if no valid response try again
			if res == -1 or res != 47 :
				res = MidMan.Z.stepper_time_bet_steps(self.Zlist[1])
				#if again no valid response, flag an error
				if res == -1 or res != 47 :
					self.end_UI()
					print "\r\nError sending stepper_time_bet_steps. Hint: check if stepper module is properly defined in Z axis controller"
					return False
			#if time between steps is sent properly then proceed to send the number of steps
			res1 = MidMan.Z.stepper_move(self.Zlist[0])
			#if no valid response try again
			if res1 == -1 or res1 != 47 :
				res1 = MidMan.Z.stepper_move(self.Zlist[0])
				#if again no valid response, flag an error
				if res1 == -1 or res1 != 47 :
					self.end_UI()
					print "\r\nError sending stepper_move. Hint: check if stepper module is properly defined in Z axis controller"
					return False
		return True

	def send_Edata(self) :
		if self.Elist[0] != 0 :
			#send time between steps
			res = MidMan.Z.ext_stepper_time_bet_steps(self.Elist[1])
			#if no valid response try again
			if res == -1 or res != 47 :
				res = MidMan.Z.ext_stepper_time_bet_steps(self.Elist[1])
				#if again no valid response, flag an error
				if res == -1 or res != 47 :
					self.end_UI()
					print "\r\nError sending ext_stepper_time_bet_steps. Hint: check if extruder stepper module is properly defined in E axis controller"
					return False
			#if time between steps is sent properly then proceed to send the number of steps
			res1 = MidMan.Z.ext_stepper_move(self.Elist[0])
			#if no valid response try again
			if res1 == -1 or res1 != 47 :
				res1 = MidMan.Z.ext_stepper_move(self.Elist[0])
				#if again no valid response, flag an error
				if res1 == -1 or res1 != 47 :
					self.end_UI()
					print "\r\nError sending ext_stepper_move. Hint: check if extruder stepper module is properly defined in Z axis controller"
					return False
		return True

	def send_heatbed_t(self) :
		if (self.heatbed_t - self.old_heatbed_t) != 0 :
			#send heatbed temperature
			res = MidMan.Y.heatbed_set_temp(self.heatbed_t)
			#if no valid response try again
			if res == -1 or res != 47 :
				res = MidMan.Y.heatbed_set_temp(self.heatbed_t)
				#if again no valid response, flag an error
				if res == -1 or res != 47 :
					self.end_UI()
					print "\r\nError sending heatbed_set_temp. Hint: check if heatbed module is properly defined in Y axis controller"
					return False
		self.old_heatbed_t = self.heatbed_t
		return True

	def send_ext_t(self) :
		if (self.ext_heat - self.old_ext_heat) != 0 :
			#send heatbed temperature
			res = MidMan.X.ext_heat_set_temp(self.ext_heat)
			#if no valid response try again
			if res == -1 or res != 47 :
				res = MidMan.X.ext_heat_set_temp(self.ext_heat)
				#if again no valid response, flag an error
				if res == -1 or res != 47 :
					self.end_UI()
					print "\r\nError sending ext_heat_set_temp. Hint: check if extruder heat module is properly defined in X axis controller"
					return False
		self.old_ext_heat = self.ext_heat
		return True

	def monitor_Xstatus(self) :
		if self.Xlist[0] != 0 :
			#get stepper status
			res = MidMan.X.stepper_status()
			#if no valid response try again
			if res == -1 :
				res = MidMan.X.stepper_status()
				#if again no valid response, flag an error
				if res == -1 :
					self.end_UI()
					print "\r\nError getting stepper_status. Hint: check if stepper module is properly defined in X axis controller"
					return False
			#process stepper status
			#print res
			statusList = res.split(",")
			for parameter in statusList :
				if 'Home endstop pressed' in parameter :
					self.end_UI()
					print "\r\nError Home endstop pressed in X axis. Hint: check print space dimensions in the slicer software"
					return False
				if 'Away endstop pressed' in parameter :
					self.end_UI()
					print "\r\nError Away endstop pressed in X axis. Hint: check print space dimensions in the slicer software"
					return False
				if "remain_steps" in parameter :
					lst = parameter.split()
					if 1 < len(lst) :
						if lst[1].isdigit() :
							self.Xremain_steps = int(lst[1])
				if "t_bet_steps" in parameter :
					lst = parameter.split()
					if 1 < len(lst) :
						if lst[1].isdigit() :
							self.Xt_bet_steps_stat = int(lst[1])
				#if 'Stepper is moving' in parameter :
					
				#if 'Stepper has ended motion' in parameter :
					
			return True

		else :
			return True

	def monitor_Ystatus(self) :
		if self.Ylist[0] != 0 :
			#get stepper status
			res = MidMan.Y.stepper_status()
			#if no valid response try again
			if res == -1 :
				res = MidMan.Y.stepper_status()
				#if again no valid response, flag an error
				if res == -1 :
					self.end_UI()
					print "\r\nError getting stepper_status. Hint: check if stepper module is properly defined in Y axis controller"
					return False
			#process stepper status
			#print res
			statusList = res.split(",")
			for parameter in statusList :
				if 'Home endstop pressed' in parameter :
					self.end_UI()
					print "\r\nError Home endstop pressed in Y axis. Hint: check print space dimensions in the slicer software"
					return False
				if 'Away endstop pressed' in parameter :
					self.end_UI()
					print "\r\nError Away endstop pressed in Y axis. Hint: check print space dimensions in the slicer software"
					return False
				if "remain_steps" in parameter :
					lst = parameter.split()
					if 1 < len(lst) :
						if lst[1].isdigit() :
							self.Yremain_steps = int(lst[1])
				if "t_bet_steps" in parameter :
					lst = parameter.split()
					if 1 < len(lst) :
						if lst[1].isdigit() :
							self.Yt_bet_steps_stat = int(lst[1])
				#if 'Stepper is moving' in parameter :
					
				#if 'Stepper has ended motion' in parameter :
					
			return True

		else :
			return True

	def monitor_Zstatus(self) :
		if self.Zlist[0] != 0 :
			#get stepper status
			res = MidMan.Z.stepper_status()
			#if no valid response try again
			if res == -1 :
				res = MidMan.Z.stepper_status()
				#if again no valid response, flag an error
				if res == -1 :
					self.end_UI()
					print "\r\nError getting stepper_status. Hint: check if stepper module is properly defined in Z axis controller"
					return False
			#process stepper status
			#print res
			statusList = res.split(",")
			for parameter in statusList :
				if 'Home endstop pressed' in parameter :
					self.end_UI()
					print "\r\nError Home endstop pressed in Z axis. Hint: check print space dimensions in the slicer software"
					return False
				if 'Away endstop pressed' in parameter :
					self.end_UI()
					print "\r\nError Away endstop pressed in Z axis. Hint: check print space dimensions in the slicer software"
					return False
				if "remain_steps" in parameter :
					lst = parameter.split()
					if 1 < len(lst) :
						if lst[1].isdigit() :
							self.Zremain_steps = int(lst[1])
				if "t_bet_steps" in parameter :
					lst = parameter.split()
					if 1 < len(lst) :
						if lst[1].isdigit() :
							self.Zt_bet_steps_stat = int(lst[1])
				#if 'Stepper is moving' in parameter :
					
				#if 'Stepper has ended motion' in parameter :
					
			return True

		else :
			return True

	def monitor_Estatus(self) :
		if self.Elist[0] != 0 :
			#get stepper status
			res = MidMan.Z.ext_stepper_status()
			#if no valid response try again
			if res == -1 :
				res = MidMan.Z.ext_stepper_status()
				#if again no valid response, flag an error
				if res == -1 :
					self.end_UI()
					print "\r\nError getting ext_stepper_status. Hint: check if extruder stepper module is properly defined in Z axis controller"
					return False
			#process stepper status
			#print res
			statusList = res.split(",")
			for parameter in statusList :
				if "remain_steps" in parameter :
					lst = parameter.split()
					if 1 < len(lst) :
						if lst[1].isdigit() :
							self.Eremain_steps = int(lst[1])
				if "t_bet_steps" in parameter :
					lst = parameter.split()
					if 1 < len(lst) :
						if lst[1].isdigit() :
							self.Et_bet_steps_stat = int(lst[1])
				#if 'Stepper is moving' in parameter :
					
				#if 'Stepper has ended motion' in parameter :
					
			return True

		else :
			return True


	def monitor_heatbed_status(self) :
		#get heatbed status
		res = MidMan.Y.heatbed_status()
		#if no valid response try again
		if res == -1 :
			res = MidMan.Y.heatbed_status()
			#if again no valid response, flag an error
			if res == -1 :
				self.end_UI()
				print "\r\nError getting heatbed_status. Hint: check if heatbed module is properly defined in Y axis controller"
				return False
		#process heatbed status
		#print res
		statusList = res.split(",")
		for parameter in statusList :
			if "temp(c)" in parameter :
				lst = parameter.split()
				if 1 < len(lst) :
					if lst[1].isdigit() :
						self.heatbed_t_stat = int(lst[1])
		return True


	def monitor_ext_h_status(self) :
		#get heatbed status
		res = MidMan.X.ext_heat_status()
		#if no valid response try again
		if res == -1 :
			res = MidMan.X.ext_heat_status()
			#if again no valid response, flag an error
			if res == -1 :
				self.end_UI()
				print "\r\nError getting ext_heat_status. Hint: check if extruder heat module is properly defined in X axis controller"
				return False
		#process heatbed status
		#print res
		statusList = res.split(",")
		for parameter in statusList :
			if "ext_temp(c)" in parameter :
				lst = parameter.split()
				if 1 < len(lst) :
					if lst[1].isdigit() :
						self.ext_t_stat = int(lst[1])
		return True

	def machine_control(self) :
		if not (self.send_Xdata() and self.send_Ydata() and self.send_Zdata() and self.send_Edata() and self.send_heatbed_t() and self.send_ext_t() ):
			return False
		else :
			ret = self.monitor_Xstatus()
			if not(ret) :
				return False
			ret = self.monitor_Ystatus()
			if not(ret) :
				return False
			ret = self.monitor_Zstatus()
			if not(ret) :
				return False
			ret = self.monitor_Estatus()
			if not(ret) :
				return False
			ret = self.monitor_heatbed_status()
			if not(ret) :
				return False
			ret = self.monitor_ext_h_status()
			if not(ret) :
				return False
			while (self.Xremain_steps != 0 or self.Yremain_steps != 0 or self.Zremain_steps != 0 or self.Eremain_steps != 0 or 
				self.heatbed_t_stat < (self.heatbed_t * 0.9) or self.ext_t_stat < (self.ext_heat * 0.85) ) :
				ret = self.monitor_Xstatus()
				if not(ret) :
					return False
				ret = self.monitor_Ystatus()
				if not(ret) :
					return False
				ret = self.monitor_Zstatus()
				if not(ret) :
					return False
				ret = self.monitor_Estatus()
				if not(ret) :
					return False
				ret = self.monitor_heatbed_status()
				if not(ret) :
					return False
				ret = self.monitor_ext_h_status()
				if not(ret) :
					return False
				s1 = "Remaining steps in X axis: " + self.Xremain_steps
				MidMan.stdscr.addstr(1, 0, s1)
				s2 = "Remaining steps in Y axis: " + self.Yremain_steps
				MidMan.stdscr.addstr(2, 0, s2)
				s3 = "Remaining steps in Z axis: " + self.Zremain_steps
				MidMan.stdscr.addstr(3, 0, s3)
				s4 = "Remaining steps in Z axis: " + self.Zremain_steps
				MidMan.stdscr.addstr(4, 0, s4)
				s5 = "Remaining steps in Extruder: " + self.Eremain_steps
				MidMan.stdscr.addstr(5, 0, s5)
				s6 = "Extruder temperature: " + self.heatbed_t_stat
				MidMan.stdscr.addstr(6, 0, s6)
				s7 = "Heatbed temperature: " + self.ext_t_stat
				MidMan.stdscr.addstr(7, 0, s7)
				MidMan.stdscr.refresh()
				sleep(0.05)
			return True


	def Detach_machine(self) :
		MidMan.X.detach()
		MidMan.Y.detach()
		MidMan.Z.detach()
		MidMan.X.stopDaemon()
		MidMan.Y.stopDaemon()
		MidMan.Z.stopDaemon()
		
