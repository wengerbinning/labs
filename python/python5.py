#!/usr/bin/env python

import curses

buf = b'\x1b[0;0mdefault.hdoc\x1b[0m\r\n\x1b[0;0mdemo.conf\x1b[0m\r\n\x1b[0;0mdemo.hdoc\x1b[0m\r\n\x1b[1;32mshell.sh\x1b[0m\r\n\x1b[0;0mdefault.hdoc\x1b[0m\r\n\x1b[0;0mdemo.conf\x1b[0m\r\n\x1b[0;0mdemo.hdoc\x1b[0m\r\n\x1b[1;32mshell.sh\x1b[0m\r\n\x1b[0;0mdefault.hdoc\x1b[0m\r\n\x1b[0;0mdemo.conf\x1b[0m\r\n\x1b[0;0mdemo.hdoc\x1b[0m\r\n\x1b[1;32mshell.sh\x1b[0m\r\n\x1b[0;0mdefault.hdoc\x1b[0m\r\n\x1b[0;0mdemo.conf\x1b[0m\r\n\x1b[0;0mdemo.hdoc\x1b[0m\r\n\x1b[1;32mshell.sh\x1b[0m\r\n\x1b[0;0mdefault.hdoc\x1b[0m\r\n\x1b[0;0mdemo.conf\x1b[0m\r\n\x1b[0;0mdemo.hdoc\x1b[0m\r\n\x1b[1;32mshell.sh\x1b[0m\r\n\x1b[0;0mdefault.hdoc\x1b[0m\r\n\x1b[0;0mdemo.conf\x1b[0m\r\n\x1b[0;0mdemo.hdoc\x1b[0m\r\n\x1b[1;32mshell.sh\x1b[0m\r\n'

def shell_format(buf):
	idx = 0
	esc = False
	ctl = False
	attr = b''
	value = b''
	attrs = list()
	content = []
	#
	while idx < len(buf):
		if buf[idx] == 0x1B:
			esc = True
			if len(value):
				content.append({'attrs':attrs, 'value':value})
				value = b''
			elif len(attrs):
				content.append({'attrs':attrs, 'value':b''})
			attrs = []
		elif esc and buf[idx] == 0x5B:
			ctl = True
		elif ctl and buf[idx] == 0x3B:
			attrs.append(attr.decode())
			attr = b''
		elif ctl and buf[idx] == 0x6D:
			attrs.append(attr.decode())
			attr = b''
			ctl = False
		elif ctl:
			attr += int.to_bytes(buf[idx])
		elif buf[idx] == 0x0D:
			pass
		elif buf[idx] == 0x0A:
			value += int.to_bytes(buf[idx])
		else:
			value += int.to_bytes(buf[idx])
		idx += 1
	if len(value):
		content.append({'attrs':attrs, 'value':value})
	elif len(attrs):
		content.append({'attrs':attrs, 'value':b''})
	return content

# content = shell_format(buf)

# for each in content:
#     print(each)



color_map = {
	'31': (curses.COLOR_RED, -1),
	'32': (curses.COLOR_GREEN, -1),
	'33': (curses.COLOR_YELLOW, -1),
	'34': (curses.COLOR_BLUE, -1),
	'35': (curses.COLOR_MAGENTA, -1),
	'36': (curses.COLOR_CYAN, -1),
	'37': (curses.COLOR_WHITE, -1),
	'1': curses.A_BOLD,
	'4': curses.A_UNDERLINE,
	'7': curses.A_REVERSE
}

# 初始化颜色对
for i, (fg, bg) in enumerate(color_map.values(), start=1):
	# if isinstance(fg, tuple):
	pass