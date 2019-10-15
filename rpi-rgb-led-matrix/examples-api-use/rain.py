from PIL import Image
import os
import sys
import urllib2 as urllib
import io

size = 50,32
if(len(sys.argv) < 2):
	im = Image.open("get.jpg")
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
im = im.crop((left, top, right, bottom))
im.save("stillShot.ppm")
os.system('sudo ./demo -D 1 stillShot.ppm --led-no-hardware-pulse --led-rows=16 --led-cols=32 --led-chain=2 --led-pixel-mapper="U-mapper" -m=0')