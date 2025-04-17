#!/usr/bin/env python

import time
import serial
import curses
import syslog
import threading

def serial_watching_worker(port):
	monitor = ConsoleMonitor(port)
	serial = monitor.serials[port]
	while True:
		if 0 < serial.in_waiting:
			buffer = serial.read(serial.in_waiting)
			# curse can't print string with '\r\n'
			# monitor.obuffer += buffer.decode('utf-8', errors='ignore')
			monitor.obuffer += buffer.decode('utf-8', errors='ignore').replace('\r\n', '\n')
			monitor.ochange = True

def curses_create_iowin (title, rows, cols, x, y):
	win = curses.newwin(rows, cols, x, y)
	win.box()
	win.addstr(0, (cols//2 - len(title)//2), title)
	row = rows - 2; col = cols - 2
	text = win.subwin(row, col, (x + 1), (y + 1))
	return win, text

def curses_console (mwin):
	monitor = ConsoleMonitor("console")
	# global setting
	curses.noecho()
	curses.curs_set(1)
	curses.start_color()
	curses.init_pair(1, curses.COLOR_BLACK, curses.COLOR_WHITE)
	# main window
	rows, cols = mwin.getmaxyx()
	mwin.keypad(True)
	mwin.nodelay(True)
	title = "Serial Monitor ({},{})".format(rows, cols)
	line = 1; offset = cols//2 - len(title)//2
	mwin.addstr(line, offset, title)
	mwin.refresh()
	# output window
	x = 2; y = 2
	row = rows//2 - 2; col = cols - 4
	owin, otext = curses_create_iowin(" Standard Output ", row, col, x, y)
	otext.idlok(True)
	otext.scrollok(True)
	owin.refresh()
	monitor.ochange = True
	# input window
	x = rows//2 - 0; y = 2
	row = rows//2 - 2; col = cols - 4
	iwin, itext = curses_create_iowin(" Standard Input ", row, col, x, y)
	itext.idlok(True)
	itext.scrollok(True)
	iwin.refresh()
	monitor.ichange = True
	stamp0 = time.time()
	while True:
		# main window
		stamp1 = time.time()
		if 0.01 < stamp1 - stamp0: # 50ms
			mrow = rows; mcol = cols
			rows, cols = mwin.getmaxyx()
			if rows <= 20 or cols <= 50:
				break
			if rows != mrow or cols != mcol:
				mwin.clear()
				title = "Serial Monitor # ({},{})".format(rows, cols)
				line = 1; offset = cols//2 - len(title)//2
				mwin.addstr(line, offset, title)
				mwin.refresh()
				# standard output window
				x = 2; y = 2
				row = rows//2 - 2; col = cols - 4
				owin, otext = curses_create_iowin(" Standard Output ", row, col, x, y)
				owin.refresh()
				ctx.ochange = True
				# standard input window
				x = rows//2 - 0; y = 2
				row = rows//2 - 2; col = cols - 4
				iwin, itext = curses_create_iowin(" Standard Input ", row, col, x, y)
				iwin.refresh()
				ctx.ichange = True
			else:
				otext.clear()
				itext.clear()
			mwin.refresh()
		# output text area window
		if monitor.ochange:
			monitor.ochange = False

			buffer = "{}# {}".format(monitor.obuffer, monitor.obuffer.count('\n'))
			syslog.syslog(syslog.LOG_WARNING, buffer)
			otext.addstr(buffer)
			# otext.addstr(1, 0, "demo")
			otext.refresh()
		# input text area window
		if monitor.ichange:
			monitor.ichange = False
			x, y = itext.getbegyx()
			itext.addstr(0, 0, monitor.iline)
			mwin.move(x, (y + len(monitor.iline)))
			itext.refresh()
		# terminal input
		try:
			c = mwin.getch()
			if c == 27:
				break
			elif c == curses.KEY_ENTER or c == 10 or c == 13:
				# monitor.obuffer += monitor.iline + "\n"
				monitor.ibuffer = monitor.iline + "\n"
				monitor.iline = ""
				monitor.ieof = True
				monitor.ochange = True; monitor.ichange = True
			elif c == curses.KEY_BACKSPACE or c == 127:
				monitor.iline = monitor.iline[:-1]
				monitor.ichange = True
			elif 32 <= c <=126:
				monitor.iline += chr(c)
				monitor.ichange = True
		except:
			break

class ConsoleMonitor:
	_instance = None

	def __connect_serial(self, port, baudrate, timeout=3):
		self.serials[port] = serial.Serial(port, baudrate, timeout=timeout)
		self.ports.append(port)


	def __new__(cls, name, port=None, baudrate=None):
		if cls._instance is None:
			cls._instance = super().__new__(cls)
			cls._instance.ports = []
			cls._instance.serials = {}
			cls._instance.workers = {}
			cls._instance.threadpools = {}
			cls._instance.obuffer = ""
			cls._instance.ibuffer = ""
			if port and baudrate:
				cls._instance.__connect_serial(port, baudrate)
		return cls._instance

	def __init__(self, name, port=None, baudrate=None):
		self.workers.update({name: {}})
		self.ochange = True
		self.iline = ""
		self.ieof = False
		self.ichange = True

	def watching(self):
		curses.wrapper(curses_console)

		# while True:
		# 	if self.ochange:
		# 		self.ochange = False
		# 		print(self.obuffer)

	def serial_watching(self, port):
		thread = threading.Thread(target=serial_watching_worker, args=(port,))
		self.threadpools.update({port:thread})
		thread.start()

	def serial_write(self, port):
		pass










if __name__ == "__main__":
	syslog.openlog(ident='monitor')
	monitor = ConsoleMonitor("main", "/dev/ttyUSB1", 57600)

	#
	for port in monitor.serials.keys():
		monitor.serial_watching(port)

	# print(monitor.workers.keys())
	# print(monitor.serials.keys())
	# print(monitor.ports)

	monitor.watching()