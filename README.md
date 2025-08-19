# brontosaurus led project in c
## VIDEO DEMO: https://youtu.be/DuefeIWOSQM

## hardware
- the Makefile uploads to an atmega328p chip on an Arduino nano
- the brontosaurus circuit has 6 74HC595 shift registers daisy-chained together driving 48 5mm LEDs
## software
- the bronto code is written in plain c without the arduino framework to keep it as lightweight as possible
- avrdude & avr-gcc are required to compile & upload to the arduino

## fully built example
![image](./z_docs/img/ref.png)

## schematic
![image](https://github.com/jdetok/bronto-arduino/blob/main/bronto_schematic.jpg?raw=true)