import sys

pngCrcTable = []
for i in range(256):
	c = i
	for k in range(8):
		if c & 1:
			c = 0xEDB88320 ^ ((c >> 1) & 0x7FFFFFFF)
		else:
			c = (c >> 1) & 0x7FFFFFFF
	pngCrcTable.append(c)

def pngcrc(data, crc=0):
	c = crc ^ 0xFFFFFFFF
	for i in data:
		c = pngCrcTable[(c ^ i) & 0xFF] ^ ((c >> 8) & 0xFFFFFF)
	return c ^ 0xFFFFFFFF

f = b"\x00\x00\x00\x02tRNS\x00\x80"
crc = pngcrc(f[4:])
f += crc.to_bytes(4, byteorder='big')
with open(sys.argv[1], "rb") as fd:
	t = fd.read()
if b'PLTE' not in t:
	exit(1)
if b'tRNS' in t:
	index = t.index(b"tRNS")
	index -= 4
	length = int.from_bytes(t[index:index + 4], byteorder='big') + 12
	with open(sys.argv[1], "wb") as fd:
		fd.write(t[:index])
		fd.write(f)
		fd.write(t[index + length:])
else:
	index = t.index(b"IDAT")
	index -= 4
	with open(sys.argv[1], "wb") as fd:
		fd.write(t[:index])
		fd.write(f)
		fd.write(t[index:])
