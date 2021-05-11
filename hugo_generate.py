#/usr/bin/env python

import json
import sys
from glob import glob
import os

with open(sys.argv[1]) as fh:
	data = json.load(fh)

heads = [x["group"] for x in data]

print ("# Links")

for h in heads:
	print (" * [{0}]({1})".format(h, "#" + h.lower()))

print ()

for d in data:
	print ("# {0}".format(d["group"]))
	for imgfn in glob(os.path.join(os.path.dirname(sys.argv[2]), d["images"])):
		imgfn = os.path.basename(imgfn)
		name = imgfn.split(".")[:-1]
		print ("![{0}]({1} \"{2}\")".format(name, imgfn, name))

	print (" [ Up ](#)")
