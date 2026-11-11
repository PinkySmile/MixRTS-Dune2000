import os
import sys
import json


source    = sys.argv[1]
tileset   = sys.argv[2]
name      = sys.argv[3]
canon_name= sys.argv[4]


with open(source, "rb") as fd:
	map_data = fd.read()
size = [
	int.from_bytes(map_data[0:2], 'little'),
	int.from_bytes(map_data[2:4], 'little')
]

starts = []
objects = []
indices = bytearray(size[0] * size[1] * 4)
for y in range(size[1]):
	for x in range(size[0]):
		offset_int = (size[0] * y + x) * 4 + 4
		offset_out = (size[0] * y + x) * 4
		value = int.from_bytes(map_data[offset_int:offset_int + 4], 'little')
		index = value & 0xFFFF
		flags = value >> 16
		px = x * 32
		py = y * 32
		if flags in [1, 2]:
			index |= flags << 24
		elif flags == 23:
			starts.append({ "x": px, "y": py })
		elif flags == 20:
			index |= 1 << 24
			objects.append({ "type": "worm", "x": px, "y": py })
		elif flags == 41:
			index |= 1 << 24
			objects.append({ "type": "bloom", "x": px, "y": py, "limit": 5 })
		elif flags == 42:
			index |= 1 << 24
			objects.append({ "type": "bloom", "x": px, "y": py, "limit": 10 })
		elif flags == 43:
			index |= 1 << 24
			objects.append({ "type": "bloom", "x": px, "y": py, "limit": 25 })
		elif flags == 44:
			index |= 1 << 24
			objects.append({ "type": "bloom", "x": px, "y": py, "limit": 50 })
		elif flags == 45:
			index |= 1 << 24
			objects.append({ "type": "bloom", "x": px, "y": py })
		elif flags != 0:
			print(f"{source}: WARNING: Unknown flag {flags} at {x},{y}")
		indices[offset_out:offset_out + 4] = index.to_bytes(4, 'little')

try:
	os.mkdir(name)
except:
	pass
with open(name + "/tiles.dat", "wb") as fd:
	fd.write(indices)
with open(name + "/map.json", "w") as fd:
	json.dump({
		"name": canon_name.strip(),
		"author": "Intelligent Games",
		"origin": "Dune 2000",
		"tileset": f"../{tileset}",
		"objects": objects,
		"starts": starts,
		"grid": { "x": 32, "y": 32 },
		"size": { "x": size[0], "y": size[1] },
		"translate": { "x": 0, "y": 0 }
	}, fd, indent='\t')
