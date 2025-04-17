#!/usr/bin/env python

import time
import serial
import serial.tools.list_ports





# ports = serial.tools.list_ports.comports()

# for port in ports:
# 	print(port)




class SerialConsole:
	def __init__(self, port, baudrate=57600):
		self.port = port
		self.baudrate = baudrate

	def connect(self):
		timeout = 3
		try:
			dev = serial.Serial(self.port, self.baudrate, timeout=3)
			print("connect to {} ({}) ...".format(self.port, self.baudrate))


			while True:
				# line = dev.readline()
				# if line:
				# 	print(line)

				if dev.in_waiting > 0:
					rbuffer = dev.read(dev.in_waiting)
					print(rbuffer.decode('utf-8', errors='ignore'), end='')

				# print("wait recv {} ...".format(dev.in_waiting))
				# time.sleep(1)





		except serial.SerialException as err:
			print(err)



	def send(self):
		pass

	def recv(self):
		pass





if __name__ == "__main__":
	# print("running in here")
	port = "/dev/ttyUSB1"
	console = SerialConsole(port)
	console.connect()
