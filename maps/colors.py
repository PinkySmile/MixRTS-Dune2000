import sys

rules = [
	[0xF7AE52, [0x1000000000000, 0]],
	[0xE77921, [0x2000000000000, 0]],
	[0x5A2829, [0x0000000000000, 0x6000]],
	[0x825537, [0x0000000004000, 0x2000]],
	[0xDC9C50, [0x0000000008000, 0]],
	[0xE4A458, [0x0000020000000, 0]],
	[0xEFDFAC, [0x0000040000000, 0]],
	[0xEFDF8C, [0x0000000000000, 0]],
]

thick_spice = [300, 301, 320, 321]
thin_spice = [*range(748, 758)] + [*range(760, 800)]

def get_rule(index, attrs):
	if index in thin_spice:
		return 0
	if index in thick_spice:
		return 1
	for k, r in enumerate(rules):
		if (attrs & r[1][0]) == r[1][0] and (attrs & r[1][1]) == 0:
			return k
	assert False

with open(sys.argv[1], "rb") as fd:
	tileattrs = fd.read()

attributes = tileattrs[0:3200]
assert len(attributes) == 3200

data = bytearray(1 + len(rules) * 3 + 800)
data[0] = len(rules)
i = 1
for r in rules:
	data[i:i + 3] = r[0].to_bytes(3, 'little')
	i += 3
for j in range(800):
	attr = int.from_bytes(attributes[j * 4:j * 4 + 4], 'little')
	data[i] = get_rule(j, attr)
	print(j, hex(attr), data[i])
	i += 1
assert len(data) == i
with open(sys.argv[2], "wb") as fd:
	fd.write(data)