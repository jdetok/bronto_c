#include <avr/io.h> 
#include <util/delay.h>
#include "../lib/shift.h"
#include "../lib/ui.h"
#include "../lib/rgb.h"
#include "../lib/analog.h"

int main() {
    // setup digital pins for shift register
    shiftReg sr = {
        .ser = 1 << PD4, // d4 serial pin
        .oe = 1 << PD6, // d6 oe pin pwm
        .latch = 1 << PD7, // d7 latch pin
        .clock = 1 << PB0, // d8 clock pin
    }; 
    // digital pins for switches & pots
    usrIn ui = {
        .intnSw = 1 << PD2, // d2 intensity switch
        .seqSw = 1 << PD3, // d3 on vs seq switch
        .pwrSw = 1 << PD5, // d5 power switch
        .revSw = 1 << PC0, // a0 alt switch - analog pin
        .rgbSw = 1 << PC5, // a5 alt switch 2 - analog pin
    };

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

        if (!getState(ui.pwrSw, 'd')) { // pwrSw off
            rgb_off();
            allBits(&sr, &ui, 6, 0); // all bits off
            continue; // break loop
        }

        // turn on rgb if A5 on
        if (getState(ui.rgbSw, 'c')) {
            rgb_on();
            pulse(&rgb, now, 1, adc_rgb_pot(4));
        } else {
            rgb_off();   
        }
        
        // run bitchaser if second switch is on, else all leds on
        if (getState(ui.seqSw, 'd')) { 
            if (getState(ui.intnSw, 'd')) { // check if intensity switch on
               bitChaser(&sr, &ui, adc_sw(3), getState(ui.revSw, 'c'));
            } else {
                bitChaser(&sr, &ui, 6, getState(ui.revSw, 'c'));
            }
        // sequence off, all lights on
        } else {
            allBits(&sr, &ui, 6, 1); // all bits on
        }
    }
}
