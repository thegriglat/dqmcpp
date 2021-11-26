#/usr/bin/env python

import json
import sys
from glob import glob
import os

with open(sys.argv[1]) as fh:
	data = json.load(fh)

heads = [x["group"] for x in data]

print ("# Links\n")

for h in heads:
	print (" * [{0}]({1})\n ".format(h, "#" + h.lower()))

print ()

for d in data:
	print ("# {0}".format(d["group"]))
	l = sorted(glob(os.path.join(os.path.dirname(sys.argv[2]), d["images"])))
	for imgfn in l:
		imgfn = os.path.basename(imgfn)
		name = imgfn.split(".")[:-1][0]
		print ("![{0}]({1})\n".format(name, imgfn))

	print (" [ Up ](#)\n")
