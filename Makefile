miru: miru.ino
	arduino-cli compile -b arduino:avr:uno
	sudo arduino-cli upload -p /dev/ttyACM0 -b arduino:avr:uno
