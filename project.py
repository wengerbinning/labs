#!/usr/bin/env python
import redis

class DBRedis:
	rdbs = {}
	pool = ["default", "account", "project"]
	def __init__(self, host, port, password=None):
		self.host = host
		self.port = port
		self.password = password

	def connect(self, db, name):
		r = redis.Redis(host=self.host, port=self.port,
			password=self.password, db=db)
		try:
			r.ping()
			self.rdbs.update({name:r})
		except redis.ConnectionError:
			print(f"connect {db} failed!")

	def connects(self):
		for db, name in enumerate(self.pool):
			self.connect(db, name)

#
db = DBRedis('localhost', 6379)
db.connects()
for db,rdb in db.rdbs.items():
	print(db)
