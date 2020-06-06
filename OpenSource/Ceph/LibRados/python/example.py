#!/usr/bin/python

import rados

try:
    cluster = rados.Rados(conffile='')
except TypeError as e:
    print "Argument validation error : ", e
    raise e;

print "Created cluster handle."

try:
    cluster.connect()
except Exception as e:
    print "connection error: ", e
    raise e
finally:
    print "Connected to cluster."

