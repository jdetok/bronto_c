# arduino pinout
# ui pins
## switches
- power switch
    - d5
- sequence/solid mode switch
    - d3
- "intensity" (intn) switch
    - d2
- rgb on switch
    - a0
- current unused switch
    - a5
## potentiometers
- brightness pot
    - a1
- speed pot
    - a2
- intensity pot
    - a3
- rgb brightness pot
    - a4
# 74HC595 shift register pins
- serial pin
    - d4
- output enable (oe) pin
    - d6
    * pwm, controls led brightness
- clock pin
    - d7
- latch pin
    - d8
# rgb pins
### all three pwm
- red
    - d9
- green
    - d10
- blue
    - d11