# arduino pinout
# ui pins
## switches
- power switch
    - d5 | PD5
- sequence/solid mode switch
    - d3 | PD3
- "intensity" (intn) switch
    - d2 | PD2
- rgb on switch 
    - a0 | PC0
- reverse chaser effect
    - a5 | PC5
## potentiometers
- shift register LED brightness pot | bottommost pot
    - a1 | PC1 | adc chan. 1
- speed pot | speed for chaser effect | second from bottom 
    - a2 | PC2 | adc chan. 2
- intensity pot | controls `num_sr` value passed to chaser | second from top
    - effectively 'adds' additional visual bits chasing as pot increases
    - a3 | PC3 | adc chan. 3
- rgb brightness pot | topmost pot
    - a4 | PC4 | adc chan. 4
# 74HC595 shift register pins
- serial pin | set a bit in the register
    - d4 | PD4
- output enable (oe) pin
    - d6 | PD6 | OCR0A = x to set shift register LED brightness
    * pwm, controls led brightness
- clock pin 
    - d7 | PD7 | set current bits to register
- latch pin
    - d8 | PB0 | output bits in shift register to memory register
# rgb pins
### all three pwm
- red
    - d9 | PB1 | OCR1A = x to set (r)gb
- green
    - d10 | PB2 | OCR1B = x to set r(g)b
- blue
    - d11 | PB3 | OCR2A = x to set rg(b)