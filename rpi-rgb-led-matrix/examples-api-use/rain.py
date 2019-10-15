from PIL import Image
import os
import sys

size = 50,32
if(len(sys.argv) < 2):
	im = Image.open("get.jpg")
else:
	image_file = sys.argv[1]
	im = Image.open(image_file)

im.thumbnail(size)
width, height = im.size
left = (width - 32)/2
top = (height - 32)/2
right = (width + 32)/2
bottom = (height + 32)/2
im = im.crop((left, top, right, bottom))
im.save("stillShot.ppm")
os.system('sudo ./demo -D 1 stillShot.ppm --led-no-hardware-pulse --led-rows=16 --led-cols=32 --led-chain=2 --led-pixel-mapper="U-mapper" -m=0')