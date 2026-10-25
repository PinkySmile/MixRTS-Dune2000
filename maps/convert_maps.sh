#!/bin/sh
FOLDER="$1"
tail +2 "$FOLDER/MultiplayerMaps.ini" | while read line; do
	FILE="$(echo $line | cut -d = -f 1)"
	NAME="$(echo $line | cut -d = -f 2)"
	if [ -f "$FOLDER/$FILE.MAP" ]; then
		MAPFILE="$FOLDER/$FILE.MAP"
	elif [ -f "$FOLDER/$FILE.map" ]; then
		MAPFILE="$FOLDER/$FILE.map"
	else
		echo "Warning: Map file not found for $FILE (checked .MAP and .map)" >&2
		continue
	fi
	python3 convert_map.py "$MAPFILE" BLOXBGBS "$FILE" "$NAME"
done
