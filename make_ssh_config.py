#! /usr/bin/env python

machines = open('/home/ubuntu/machines')
for m in machines:
	print 'Host', m[:-1]
	print '\tUser ubuntu'
	print '\tIdentityFile /home/ubuntu/mongodb.pem'
