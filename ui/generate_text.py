import sys
import json
from PIL import Image


def main(text, out):
	font_img = Image.open("font/font.png")
	with open("font/font.json") as fd:
		font_json = json.load(fd)
	w = 0
	for c in text:
		v = font_json.get(c, [0, 0])
		w += v[1] + 1
	result = Image.new("RGBA", (w, font_img.size[1]))
	w = 0
	for c in text:
		v = font_json.get(c, [0, 0])

		# 1. Define the crop box (left, upper, right, lower)
		# v[0] is X offset, v[1] is the character width
		crop_box = (v[0], 0, v[0] + v[1], font_img.size[1])

		# 2. Crop the individual character out of the sheet
		char_img = font_img.crop(crop_box)

		# 3. Paste the cropped character into the final image at the current width offset
		result.paste(char_img, (w, 0))

		w += v[1] + 1
	result.save(out)


if __name__ == "__main__":
	if len(sys.argv) < 3:
		print("Usage: python3 script.py 'Your Text Here' file.png")
		sys.exit(1)
	main(sys.argv[1], sys.argv[2])
