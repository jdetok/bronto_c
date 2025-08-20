#include <avr/io.h> 
#include <util/delay.h>
#include "../lib/shift.h"
#include "../lib/ui.h"
#include "../lib/rgb.h"
#include "../lib/analog.h"

int main() {
    // setup digital pins (d4, d6, d7, d8) for shift register
    shiftReg sr;
    shift_init(&sr);

    // digital pins for switches & pots
    switches sw;
    switch_init(&sw);

    // set digital pins as input (&=) or output (|=) 
    DDRD |= sr.ser | sr.oe | sr.latch; // d register output pins
    DDRB |= sr.clock; // b reg output pins

    // d6 serial pin on shift register pwm setup for brightness control
    oe_pwm();

    // setup digital rgb pins - red d9 OCR1A | green d10 OCR1B| blue d11 OCR2A
    rgbLED rgb;
    rgb_pwm(&rgb);

    // setup analog pins (ADC0 - ADC5) A0-A5 on arduinos
    adc_init();

    // start timing for pwm
    uint32_t now = 0;

    // MAIN LOOP
    while (1) {           
        now++; // increment timing

        if (!getState(sw.pwrSw, 'd')) { // pwrSw off
            rgb_off();
            allBits(&sr, &sw, 6, 0); // all bits off
            continue; // break loop
        }

        // turn on rgb if A5 on
        if (getState(sw.rgbSw, 'c')) {
            rgb_on();
            pulse(&rgb, now, 1, adc_rgb_pot(4));
        } else {
            rgb_off();   
        }
        
        // run bitchaser if second switch is on, else all leds on
        if (getState(sw.seqSw, 'd')) { 
            if (getState(sw.intnSw, 'd')) { // check if intensity switch on
               bitChaser(&sr, &sw, adc_sw(3), getState(sw.revSw, 'c'));
            } else {
                bitChaser(&sr, &sw, 6, getState(sw.revSw, 'c'));
            }
        // sequence off, all lights on
        } else {
            allBits(&sr, &sw, 6, 1); // all bits on
        }
    }
}
