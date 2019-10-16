#!/usr/bin/env python
import time
import sys
from rgbmatrix import RGBMatrix, RGBMatrixOptions
from PIL import Image
import os
import urllib2 as urllib
import io
from samplebase import SampleBase

class StillViewer(SampleBase):
    def __init__(self, *args, **kwargs):
        super(StillViewer, self).__init__(*args, **kwargs)
        self.parser.add_argument("-i", "--image", help="The image to display", default="../../../examples-api-use/runtext.ppm")

    
    def run(self):
        offset_canvas = self.matrix.CreateFrameCanvas()
        while True:
            size = 50,32
            # if(len(sys.argv) < 2):
            # 	im = Image.open("../media/catnaps.jpg")
            # else:
            # 	sysArg = sys.argv[1]
            # 	if sysArg.find("http") >= 0 :
            # 		fd = urllib.urlopen(sysArg)
            # 		image_file = io.BytesIO(fd.read())
            # 		im = Image.open(image_file)
            # 	else:
            # 		im = Image.open(sysArg)

            # im.thumbnail(size)
            # width, height = im.size
            # left = (width - 32)/2
            # top = (height - 32)/2
            # right = (width + 32)/2
            # bottom = (height + 32)/2
            # image = im.crop((left, top, right, bottom))


            # options = RGBMatrixOptions()
            # options.pixel_mapper_config = 'U-mapper'
            # options.rows = 16
            # options.cols = 32
            # options.chain_length = 2
            # options.parallel = 1
            # options.hardware_mapping = 'regular'

            # matrix = RGBMatrix(options = options)

            # image.thumbnail((matrix.width, matrix.height), Image.ANTIALIAS)

            # matrix.SetImage(image.convert('RGB'))

# Main function
# e.g. call with
#  sudo ./still-viewer.py --chain=4
# if you have a chain of four
if __name__ == "__main__":
    still_viewer = StillViewer()
    if (not still_viewer.process()):
        still_viewer.print_help()



# size = 50,32
# if(len(sys.argv) < 2):
# 	im = Image.open("../media/catnaps.jpg")
# else:
# 	sysArg = sys.argv[1]
# 	if sysArg.find("http") >= 0 :
# 		fd = urllib.urlopen(sysArg)
# 		image_file = io.BytesIO(fd.read())
# 		im = Image.open(image_file)
# 	else:
# 		im = Image.open(sysArg)

# im.thumbnail(size)
# width, height = im.size
# left = (width - 32)/2
# top = (height - 32)/2
# right = (width + 32)/2
# bottom = (height + 32)/2
# image = im.crop((left, top, right, bottom))


# options = RGBMatrixOptions()
# options.pixel_mapper_config = 'U-mapper'
# options.rows = 16
# options.cols = 32
# options.chain_length = 2
# options.parallel = 1
# options.hardware_mapping = 'regular'

# matrix = RGBMatrix(options = options)

# image.thumbnail((matrix.width, matrix.height), Image.ANTIALIAS)

# matrix.SetImage(image.convert('RGB'))

# try:
#     print("Press CTRL-C to stop.")
#     while True:
#         time.sleep(100)
# except KeyboardInterrupt:
#     sys.exit(0)