import sys
from PIL import Image

img1 = Image.open(sys.argv[1])
img2 = Image.open(sys.argv[2]).copy()
px1 = img1.load()
px2 = img2.load()
assert img1.size == img2.size
for x in range(img1.size[0]):
	for y in range(img2.size[1]):
		r1, g1, b1, a1 = px1[x, y]
		r2, g2, b2, a2 = px2[x, y]
		if abs(r1 - r2) + abs(g1 - g2) + abs(b1 - b2) < 20:
			px2[x, y] = (0, 0, 0, 0)
		else:
			px2[x, y] = (r1, g1, b1, 128)
img2.save(sys.argv[3])
