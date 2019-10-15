from PIL import Image
size = 320,32
im = Image.open("get.jpg")
im.thumbnail(size)
width, height = im.size
left = (width - 32)/2
top = (height - 32)/2
right = (width + 32)/2
bottom = (height + 32)/2
im = im.crop((left, top, right, bottom))
im.save("getit.ppm")