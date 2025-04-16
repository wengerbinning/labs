#!/usr/bin/env python

import serial
import serial.tools.list_ports





# ports = serial.tools.list_ports.comports()

# for port in ports:
# 	print(port)




class SerialConsole:
	def __init__(self, dev):
		pass

	def connect(self):
		try:
			var = serial.Serial(self.dev, self.baudrate, timeout)
		except serial.SerialException as err:
			print(err)

	def send(self):
		pass

	def recv(self):
		pass





if __name__ == "__main__":
	# print("running in here")
	dev = "/dev/ttyUSB2"
	console = SerialConsole(dev)
	console.connect()
	connsole.