#!/usr/bin/env python

import time
import curses
import threading
from functools import partial

def curses_create_iowin (title, rows, cols, x, y):
	win = curses.newwin(rows, cols, x, y)
	win.box()
	win.addstr(0, (cols//2 - len(title)//2), title)
	row = rows - 2; col = cols - 2
	text = win.subwin(row, col, (x + 1), (y + 1))
	return win, text

def curses_main0 (mwin):
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
	# otext.refresh()
	obuffer = ""; ochange = True
	# input window
	x = rows//2 - 0; y = 2
	row = rows//2 - 2; col = cols - 4
	iwin, itext = curses_create_iowin(" Standard Input ", row, col, x, y)
	itext.idlok(True)
	itext.scrollok(True)
	iwin.refresh()
	# itext.refresh()
	iline = ""; ichange = True
	stamp0 = time.time()
	while True:
		# main window
		stamp1 = time.time()
		if 0.05 < stamp1 - stamp0: # 50ms
			mrow=rows; mcol=cols
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
				ochange = True
				# standard input window
				x = rows//2 - 0; y = 2
				row = rows//2 - 2; col = cols - 4
				iwin, itext = curses_create_iowin(" Standard Input ", row, col, x, y)
				iwin.refresh()
				ichange = True
				# itext.refresh()
			else:
				otext.clear()
				itext.clear()
			mwin.refresh()
		# output text area window
		if ochange:
			ochange = False
			otext.addstr(0, 0, obuffer)
			otext.refresh()
		# input text area window
		if ichange:
			ichange = False
			x, y = itext.getbegyx()
			itext.addstr(0, 0, iline)
			mwin.move(x, (y + len(iline)))
			itext.refresh()

		# terminal input
		try:
			c = mwin.getch()
			if c == 27:
				break
			elif c == curses.KEY_ENTER or c == 10 or c == 13:
				obuffer += iline + "\n"
				iline = ""
				ochange = True; ichange = True
			elif c == curses.KEY_BACKSPACE or c == 127:
				iline = iline[:-1]
				ichange = True
			elif 32 <= c <=126:
				iline += chr(c)
				ichange = True
		except:
			break

def curses_main1 (mwin, ctx):
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
	ctx.ochange = True
	# input window
	x = rows//2 - 0; y = 2
	row = rows//2 - 2; col = cols - 4
	iwin, itext = curses_create_iowin(" Standard Input ", row, col, x, y)
	itext.idlok(True)
	itext.scrollok(True)
	iwin.refresh()
	ctx.ichange = True
	stamp0 = time.time()
	while True:
		# main window
		stamp1 = time.time()
		if 0.05 < stamp1 - stamp0: # 50ms
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
		if ctx.ochange:
			ctx.ochange = False
			otext.addstr(0, 0, ctx.obuffer)
			otext.refresh()
		# input text area window
		if ctx.ichange:
			ctx.ichange = False
			x, y = itext.getbegyx()
			itext.addstr(0, 0, ctx.iline)
			mwin.move(x, (y + len(ctx.iline)))
			itext.refresh()
		# terminal input
		try:
			c = mwin.getch()
			if c == 27:
				break
			elif c == curses.KEY_ENTER or c == 10 or c == 13:
				ctx.obuffer += ctx.iline + "\n"
				ctx.ibuffer = ctx.iline + "\n"
				ctx.iline = ""
				ctx.ieof = True
				ctx.ochange = True; ctx.ichange = True
			elif c == curses.KEY_BACKSPACE or c == 127:
				ctx.iline = ctx.iline[:-1]
				ctx.ichange = True
			elif 32 <= c <=126:
				ctx.iline += chr(c)
				ctx.ichange = True
		except:
			break






def task_device(name, ctx):
	pass

def task_log(name, ctx):

	f = open('file.txt', 'w')
	while True:
		if not ctx.running:
			break
		if ctx.ieof:
			ctx.ieof = False
			f.write("{}\n".format(ctx.ibuffer))

	f.close()


class ConsoleMonitor:
	obuffer = ""
	ochange = True
	ibuffer = ""
	iline = ""
	ieof = False
	ichange = True
	threads = {}
	running = True

	def __init__(self):
		thread = threading.Thread(target=task_log, args=("log", self))
		self.threads.update({'log': thread})



	def watch(self):
		curses.wrapper(partial(curses_main1, ctx=self))





if __name__ == "__main__":
	monitor = ConsoleMonitor()
	monitor.threads["log"].start()

	#
	monitor.watch()

	monitor.running = False
	monitor.threads["log"].join()
