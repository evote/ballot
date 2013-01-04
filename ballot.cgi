#!/usr/bin/env python
import sys,os
import json,yaml
import pyballot; ballot=pyballot.Ballot('/tmp/')

data = sys.stdin.read()

if os.environ["QUERY_STRING"] == '':
	pass
elif os.environ["QUERY_STRING"] == 'json':
	print 'Content-Type: application/json'
	print ''
	print json.dumps(yaml.load(ballot(yaml.dump(json.loads(data)))))
