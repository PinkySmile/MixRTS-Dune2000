import json
import sys
from PIL import Image

with open(sys.argv[1], "r") as fd:
	data = json.load(fd)
base = int(sys.argv[2])
for key in sys.argv[3:]:
	for i in data[key]:
		i['offset']['x'] = base - i['offset']['x']
		i['offset']['y'] = base - i['offset']['y']
with open(sys.argv[1], "w") as fd:
	fd.write('{\n')
	keys = list(data.keys())
	for i, key in enumerate(keys):
		l = data[key]
		if isinstance(l, list):
			fd.write(f'\t{json.dumps(key)}: [\n')
			for i2, v in enumerate(l):
				fd.write('\t\t{\n')
				keys2 = list(v.keys())
				for i3, key2 in enumerate(keys2):
					dmp = json.dumps(v[key2]).replace('{', '{ ').replace('}', ' }')
					if i3 == len(keys2) - 1:
						fd.write(f'\t\t\t{json.dumps(key2)}: {dmp}\n')
					else:
						fd.write(f'\t\t\t{json.dumps(key2)}: {dmp},\n')
				if i2 == len(l) - 1:
					fd.write('\t\t}\n')
				else:
					fd.write('\t\t},\n')
			if i == len(keys) - 1:
				fd.write('\t]\n')
			else:
				fd.write('\t],\n')
		else:
			fd.write(f'\t{json.dumps(key)}: {{\n')
			keys2 = list(l.keys())
			for i3, key2 in enumerate(keys2):
				dmp = json.dumps(l[key2]).replace('{', '{ ').replace('}', ' }')
				if i3 == len(keys2) - 1:
					fd.write(f'\t\t{json.dumps(key2)}: {dmp}\n')
				else:
					fd.write(f'\t\t{json.dumps(key2)}: {dmp},\n')
			if i == len(keys) - 1:
				fd.write('\t}\n')
			else:
				fd.write('\t},\n')
	fd.write('}\n')

#with open(sys.argv[1], "w") as fd:
#	json.dump(data, fd, indent="\t")
