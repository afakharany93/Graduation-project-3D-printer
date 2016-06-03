from time import sleep
import curses

class A:
	stdscr = curses.initscr()


A.stdscr.keypad(1)
curses.noecho()
A.stdscr.addstr(0, 0, "gm")
for i in range(10):
	s = "hello "+str(i)
	A.stdscr.addstr(1, 0, s)
	A.stdscr.refresh()
	sleep(0.5)
print("\r\nend")
sleep(0.7)
A.stdscr.keypad(0)
curses.echo()
curses.nocbreak()
curses.endwin()                 # Terminate curses

print "gotcha"
