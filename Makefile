MCU = atmega328p
F_CPU = 16000000UL
CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os
PORT = /dev/cu.usbserial-110

SRC = $(wildcard src/*.c lib/*.c)
OBJ = $(SRC:.c=.o)

TARGET = bld/main

all: $(TARGET).hex

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

$(TARGET).hex: $(TARGET)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

upload: $(TARGET).hex
	avrdude -c arduino -p m328p -P $(PORT) -b 115200 -U flash:w:$<:i

clean:
	rm -f src/*.o lib/*.o bld/*