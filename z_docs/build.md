# Makefile
- build: make
- build & upload: make upload
- remove build artifacts make clean
## ALIAS: add to .zshrc
- mumc: make upload && make clean


## build commands to manually build project & upload to arduino
## see usb port:
`ls /dev/tty.*`
- should be like `/dev/tty.usbmodem2101`
## compile to object .o file
`avr-gcc -mmcu=atmega328p -Os -DF_CPU=16000000UL -c -o bld/main.o src/main.c`
## compile to binary
`c`
## compile to hex
`avr-objcopy -O  ihex -R .eeprom bin/main hex/main.hex`
## upload to chip
- uno (with usb-b):
    - `avrdude -c arduino -p m328p -P /dev/tty.usbmodem2101 -b 115200 -U flash:w:main.hex:i`
    - `avrdude -c arduino -p m328p -P /dev/tty.usbmodem2101 -b 115200 -U flash:w:hex/main.hex:i`
- nano (with usb-c)
    - `avrdude -c arduino -p m328p -P /dev/tty.usbserial-210 -b 115200 -U flash:w:main.hex:i`

## original shell script to build & upload, replaced with Makefile
```bash
#!/usr/bin/env bash

# compile to binary & hex, upload to arduino at connected port

USBPORT=/dev/cu.usbserial-110
CHIP=atmega328p

# compile to object
avr-gcc -mmcu=$CHIP -Os -DF_CPU=16000000UL -c -o bld/main.o src/main.c
avr-gcc -mmcu=$CHIP -Os -DF_CPU=16000000UL -c -o bld/shift.o lib/shift.c

# executable
avr-gcc -mmcu=$CHIP bld/main.o bld/shift.o -o bld/main 
# avr-gcc -mmcu=$CHIP bld/main.o -o bld/main 

# hex
avr-objcopy -O  ihex -R .eeprom bld/main bld/main.hex

# upload to arduino
avrdude -c arduino -p m328p -P $USBPORT -b 115200 -U flash:w:bld/main.hex:i -v -v
```