#!/usr/bin/env python
import time
import sys
from rgbmatrix import RGBMatrix, RGBMatrixOptions
from PIL import Image
import os
import urllib2 as urllib
import io

size = 50,32
if(len(sys.argv) < 2):
	im = Image.open("../media/catnaps.jpg")
else:
	sysArg = sys.argv[1]
	if sysArg.find("http") >= 0 :
		fd = urllib.urlopen(sysArg)
		image_file = io.BytesIO(fd.read())
		im = Image.open(image_file)
	else:
		im = Image.open(sysArg)

im.thumbnail(size)
width, height = im.size
left = (width - 32)/2
top = (height - 32)/2
right = (width + 32)/2
bottom = (height + 32)/2
image = im.crop((left, top, right, bottom))


# Configuration for the matrix
options = RGBMatrixOptions()
options.pixel_mapper_config = 'U-mapper'
options.rows = 16
options.cols = 32
options.chain_length = 2
options.parallel = 1
options.hardware_mapping = 'regular'  # If you have an Adafruit HAT: 'adafruit-hat'

matrix = RGBMatrix(options = options)

# Make image fit our screen.
image.thumbnail((matrix.width, matrix.height), Image.ANTIALIAS)

matrix.SetImage(image.convert('RGB'))

try:
    print("Press CTRL-C to stop.")
    while True:
        time.sleep(100)
except KeyboardInterrupt:
    sys.exit(0)