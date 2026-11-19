import sys
import json

with open(sys.argv[1], "rb") as fd:
	data = fd.read()

name = sys.argv[2]
attributes = data[0:3200]
text_display = data[3200:6400]

GRIDMASK_HAS_CONCRETE      = 0x00100000
GRIDMASK_IS_FREE           = 0x00080000
GRIDMASK_BUILDABLE         = 0x00040000
# GRIDMASK_BUILDABLE         = 0x10000000
GRIDMASK_NO_CRATER         = 0x00000000
GRIDMASK_CRATER_TYPE_1     = 0x00010000
GRIDMASK_CRATER_TYPE_2     = 0x00020000
GRIDMASK_CRATER_TYPE_3     = 0x00030000
GRIDMASK_CRATER_TYPE       = 0x00030000
GRIDMASK_TERRAIN_TYPE      = 0x0000F000
GRIDMASK_SHIP_CAN_PASS     = 0x00000008
GRIDMASK_WORM_CAN_PASS     = 0x00000004
GRIDMASK_VEHICULES_CAN_PASS= 0x00000002
GRIDMASK_INFANTRY_CAN_PASS = 0x00000001

assert len(attributes) == 3200
assert len(text_display) == 3200
result = bytearray(800 * 4)
for i in range(800):
	a = int.from_bytes(attributes[i * 4:i * 4 + 4], 'little')
	mask = 0
	if a & 0x10000:
		mask |= GRIDMASK_WORM_CAN_PASS | GRIDMASK_CRATER_TYPE_2
	if a & 0x8000:
		mask |= GRIDMASK_BUILDABLE | GRIDMASK_CRATER_TYPE_1
	if a & 0x4000:
		mask |= GRIDMASK_INFANTRY_CAN_PASS
	if a & 0x2000:
		mask |= GRIDMASK_VEHICULES_CAN_PASS
	mask |= GRIDMASK_IS_FREE
	terrain_type = (a >> 29) & 7
	mask |= terrain_type << 12
	result[i * 4:i * 4 + 4] = mask.to_bytes(4, 'little')

with open(name + "/tileattrs.dat", "wb") as fd:
	fd.write(result)
with open(name + "/metadata.json", "w") as fd:
	json.dump({
		"map_colors": "colors.dat",
		"faction": [ "maps/objects/factions.json" ],
		"resources": [
			{
				"strategy": "random",
				"tile_pool": [300, 301, 320, 321],
				"lots": 2,
				"storage": 1,
				"id": 2,
				"deplete": 1,
				"value": { "base:credit": 100 },
				"only_adjacent_collectible": [300, 301, 320, 321]
			},
			{
				"strategy": "connected",
				"tiles": "spice.dat",
				"lots": 2,
				"storage": 1,
				"id": 1,
				"deplete": 0,
				"value": { "base:credit": 100 }
			}
		],
		"skirts": {
			"2x2": [
				617, 618,
				637, 638
			],
			"3x2": [
				611, 612, 613,
				631, 632, 633
			],
			"3x3": [
				614, 615, 616,
				634, 635, 636,
				654, 655, 656
			]
		},
		"skirts_conc": {
			"2x2": [
				649, 650,
				669, 670
			],
			"3x2": [
				643, 644, 645,
				663, 664, 665
			],
			"3x3": [
				646, 647, 648,
				666, 667, 668,
				686, 687, 688
			]
		}
	}, fd, indent='\t')
