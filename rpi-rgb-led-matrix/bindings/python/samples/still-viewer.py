#!/usr/bin/env python
from rgbmatrix import RGBMatrix, RGBMatrixOptions
from PIL import Image
import urllib2 as urllib
import io
from samplebase import SampleBase

class StillViewer(SampleBase):
    def __init__(self, *args, **kwargs):
        super(StillViewer, self).__init__(*args, **kwargs)
        self.parser.add_argument("-i", "--image", help="The image to display", default="../media/catnaps.jpg")
    
    def run(self):
        offset_canvas = self.matrix.CreateFrameCanvas()
        while True:
            size = 50,32
            sysArg = self.args.image
            if sysArg.find("http") >= 0 :
            	fd = urllib.urlopen(sysArg)
            	image_file = io.BytesIO(fd.read())
            	image = Image.open(image_file).convert('RGB')
            else:
            	image = Image.open(sysArg).convert('RGB')

            self.image.resize((self.matrix.width, self.matrix.height), Image.ANTIALIAS)            
            self.matrix.SetImage(image)

# Main function
# e.g. call with
#  sudo ./still-viewer.py --chain=4
# if you have a chain of four
if __name__ == "__main__":
    still_viewer = StillViewer()
    if (not still_viewer.process()):
        still_viewer.print_help()