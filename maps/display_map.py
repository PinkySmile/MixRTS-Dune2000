import sys
import random
from PIL import Image


#  1 -> Spice
#  2 -> Dense spice
# 20 -> Worm spawner
# 23 -> Player start
# 41 -> Spice Bloom (5 times)
# 42 -> Spice Bloom (10 times)
# 43 -> Spice Bloom (25 times)
# 44 -> Spice Bloom (50 times)
# 45 -> Spice Bloom (infinite)


def get_dir(size, x, y):
	value = 0
	value |= int(get_tile(size, x - 1, y + 1, (748, 0, 0))[0] in spices_) << 0 # 0x01 Bottom Left
	value |= int(get_tile(size, x,     y + 1, (748, 0, 0))[0] in spices_) << 1 # 0x02 Bottom
	value |= int(get_tile(size, x + 1, y + 1, (748, 0, 0))[0] in spices_) << 2 # 0x04 Bottom Right
	value |= int(get_tile(size, x - 1, y,     (748, 0, 0))[0] in spices_) << 3 # 0x08 Left
	value |= int(get_tile(size, x + 1, y,     (748, 0, 0))[0] in spices_) << 4 # 0x10 Right
	value |= int(get_tile(size, x - 1, y - 1, (748, 0, 0))[0] in spices_) << 5 # 0x20 Top Left
	value |= int(get_tile(size, x,     y - 1, (748, 0, 0))[0] in spices_) << 6 # 0x40 Top
	value |= int(get_tile(size, x + 1, y - 1, (748, 0, 0))[0] in spices_) << 7 # 0x80 Top Right
	return value


def get_tile(size, x, y, default = (0, 0, 0)):
	if x < 0 or x >= size[0] or y < 0 or y >= size[1]:
		return default
	offset = 4 + (size[0] * y + x) * 4
	value = int.from_bytes(data[offset:offset + 4], 'little')
	index = value & 0xFFFF
	flags = value >> 16
	g = None
	if flags == 1:
		index = 748
	elif flags == 2:
		index = random.choice(spices)
	elif flags == 20:
		g = 6
	elif flags == 23:
		g = 5
	elif flags == 41:
		g = 1
		index = 748
	elif flags == 42:
		g = 2
		index = 748
	elif flags == 43:
		g = 3
		index = 748
	elif flags == 44:
		g = 4
		index = 748
	elif flags == 45:
		g = 0
		index = 748
	elif flags != 0:
		g = 7
	return index, flags, g


def get_index_from_dir(dir):
	# Single island
	if (dir & 0x5A) == 0:
		return 794

	# Connected island (1)
	elif (dir & 0x5A) == 0x02:
		return 764
	elif (dir & 0x5A) == 0x08:
		return 767
	elif (dir & 0x5A) == 0x10:
		return 766
	elif (dir & 0x5A) == 0x40:
		return 765

	# 2 island (no diag)
	elif (dir & 0x5A) == 0x42:
		return 791
	elif (dir & 0x5A) == 0x18:
		return 792
	elif (dir & 0x5E) == 0x12:
		return 750
	elif (dir & 0x5B) == 0x0A:
		return 751
	elif (dir & 0xDA) == 0x50:
		return 752
	elif (dir & 0x7A) == 0x48:
		return 753
	# 2 island (diag)
	elif (dir & 0x5E) == 0x16:
		return 760
	elif (dir & 0x5B) == 0x0B:
		return 761
	elif (dir & 0xDA) == 0xD0:
		return 762
	elif (dir & 0x7A) == 0x68:
		return 763

	# 3 island (no diag)
	elif (dir & 0xDE) == 0x52:
		return 754
	elif (dir & 0x7B) == 0x4A:
		return 755
	elif (dir & 0x5F) == 0x1A:
		return 756
	elif (dir & 0xFA) == 0x58:
		return 757
	# 3 island (1 diag)
	elif (dir & 0xDE) == 0xD2:
		return 776
	elif (dir & 0x7B) == 0x6A:
		return 777
	elif (dir & 0x5F) == 0x1B:
		return 778
	elif (dir & 0x5F) == 0x1E:
		return 779
	elif (dir & 0xDE) == 0x56:
		return 796
	elif (dir & 0x7B) == 0x4B:
		return 797
	elif (dir & 0xFA) == 0x78:
		return 798
	elif (dir & 0xFA) == 0xD8:
		return 799
	# 3 island (2 diag)
	elif (dir & 0x5F) == 0x1F:
		return 768
	elif (dir & 0x7B) == 0x6B:
		return 769
	elif (dir & 0xDE) == 0xD6:
		return 770
	elif (dir & 0xFA) == 0xF8:
		return 771

	# 4 island (no diag)
	elif dir == 0x5A:
		return 790
	# 4 island (1 diag)
	elif dir == 0x7A:
		return 780
	elif dir == 0xDA:
		return 781
	elif dir == 0x5B:
		return 782
	elif dir == 0x5E:
		return 783
	# 4 island (2 diag)
	elif dir == 0x7B:
		return 784
	elif dir == 0x5F:
		return 785
	elif dir == 0x7E:
		return 786
	elif dir == 0xDE:
		return 787
	elif dir == 0xFA:
		return 788
	elif dir == 0xDB:
		return 789
	# 4 island (3 diag)
	elif dir == 0xDF:
		return 772
	elif dir == 0x7F:
		return 773
	elif dir == 0xFE:
		return 774
	elif dir == 0xFB:
		return 775
	return 748


def get_display_tile(size, x, y):
	index, flags, g = get_tile(size, x, y)
	if index == 748:
		index = get_index_from_dir(get_dir(size, x, y))

	return index, flags, g


if __name__ == '__main__':
	extra = Image.open("unconverted/extra.png")
	tileset = Image.open(sys.argv[2] + "/tilesheet.png")
	with open(sys.argv[2] + "/colors.dat", "rb") as fd:
		colors_data = fd.read()
		color_list = []
		for i in range(colors_data[0]):
			color_list.append((colors_data[1+i*3 + 2], colors_data[1+i*3 + 1], colors_data[1+i*3]))
		colors = [color_list[k] for k in colors_data[1+colors_data[0]*3:]]
		assert len(colors) == 800
	with open(sys.argv[1], "rb") as fd:
		data = fd.read()
	size = [
		int.from_bytes(data[0:2], 'little'),
		int.from_bytes(data[2:4], 'little')
	]
	result = Image.new("RGB", (size[0] * 32, size[1] * 32))
	result_mm = Image.new("RGB", (size[0], size[1]))
	px = result_mm.load()
	spices = [300, 301, 320, 321]
	spices_ = [300, 301, 320, 321, 748]
	for y in range(size[1]):
		for x in range(size[0]):
			index, flags, g = get_display_tile(size, x, y)
			f = tileset.crop((index % 20 * 32, index // 20 * 32, index % 20 * 32 + 32, index // 20 * 32 + 32))
			result.paste(f, (x * 32, y * 32))
			if g is not None:
				g = extra.crop((g * 32, 0, g * 32 + 32, 32))
				result.paste(g, (x * 32, y * 32), g)
			offset = 4 + (size[0] * y + x) * 4
			px[x,y] = colors[index]
	if len(sys.argv) == 3:
		result.show()
		result_mm.show()
	else:
		result.save(sys.argv[3])
		result_mm.save("mini_" + sys.argv[3])
