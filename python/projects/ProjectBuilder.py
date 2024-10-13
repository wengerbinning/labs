import os
import subprocess
import sys
import os
import time

class ExecException(Exception):
	def __init__(self, cmd, code):
		self.cmd = cmd
		self.code = code

	def __str__(self):
		return "Command:\"\033[32m{0}\033[0m\", Error Code:\033[31m {1}\033[0m".format(self.cmd, self.code)

class Worker:
	idx = 0

	def __init__(self, cwd="./"):
		self.cwd = cwd

	def __enter__(self):
		self.old_cwd = os.getcwd()
		os.chdir(self.cwd)
		return self

	def __exit__(self, exc_type, exc_val, exc_tb):
		os.chdir(self.old_cwd)
		return True

	def do(self, cmd):
		self.idx += 1
		print("\033[32m{0}.# {1}\033[0m".format(self.idx, cmd))
		proc = subprocess.Popen(cmd, shell=True, bufsize=0, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
		while proc.poll() is None:
			if not proc.stdin.closed:
				proc.stdin.close()
			while line := proc.stdout.readline():
				print("\033[32m >\033[0m", line.decode('utf-8'), end='')
		if 0 != proc.returncode:
			raise ExecException(cmd, proc.returncode)

class ProjectBuilder:
	def __init__(self, proj, commands, verbose=False):
		self.project = proj
		self.commands = commands
		self.verbose = verbose

	def run(self):
		with Worker(self.project.get("path")) as worker:
			for cmd in self.commands:
				worker.do(cmd)