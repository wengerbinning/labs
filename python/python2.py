#!/usr/bin/env python

import time
import serial
import threading

#
# __new__: 静态类方法,
# __init__: 对象方法
#

def serial_watching_worker(port):
	monitor = ConsoleMonitor(port)
	serial = monitor.serials[port]
	while True:
		if 0 < serial.in_waiting:
			buffer = serial.read(serial.in_waiting)
			# print("[{}]:".format(port), buffer.decode('utf-8', errors='ignore'), end='')
			monitor.obuffer += buffer.decode('utf-8', errors='ignore')

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
			if port and baudrate:
				cls._instance.__connect_serial(port, baudrate)
		return cls._instance

	def __init__(self, name, port=None, baudrate=None):
		self.workers.update({name: {}})

	def watching(self):
		while True:
			time.sleep(1)
			print(monitor.obuffer, end='')

	def serial_watching(self, port):
		thread = threading.Thread(target=serial_watching_worker, args=(port,))
		self.threadpools.update({port:thread})
		thread.start()

	def serial_write(self, port):
		pass










if __name__ == "__main__":
	monitor = ConsoleMonitor("main", "/dev/ttyUSB1", 57600)

	#
	for port in monitor.serials.keys():
		monitor.serial_watching(port)

	# print(monitor.workers.keys())
	# print(monitor.serials.keys())
	# print(monitor.ports)



	monitor.watching()