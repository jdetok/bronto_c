#include <avr/io.h> 
#include <util/delay.h>
#include "../lib/shift.h"
#include "../lib/ui.h"
#include "../lib/rgb.h"

int main() {
    // setup digital pins for shift register
    struct shiftReg sr = {
        1 << PD4, // d4 serial pin
        1 << PD6, // d6 oe pin - also initialized with pwm to control brightness
        1 << PD7, // d7 latch pin
        1 << PB0, // d8 clock pin
    }; // digital pins for switches & pots
    struct usrIn ui = {
        1 << PD2, // d2 intensity switch
        1 << PD3, // d3 on vs seq switch
        1 << PD5, // d5 power switch
        1 << PC0, // a0 alt switch - analog pin
        1 << PC5, // a5 alt switch 2 - analog pin
    };
    
    // set digital pins as input (&=) or output (|=) 
    DDRD |= sr.ser | sr.oe | sr.latch; // d register output pins
    DDRB |= sr.clock; // b reg output pins
    DDRD &= ~ui.pwrSw | ~ui.seqSw | ~ui.intnSw; // d reg input pins

    // d6 serial pin on shift register pwm setup for brightness control
    pwm_init_d6(); 

    // setup digital rgb pins - red d9 OCR1A | green d10 OCR1B| blue d11 OCR2A
    struct rgbLED rgb;
    rgb_pwm(&rgb);

    // setup analog pins (ADC0 - ADC5) A0-A5 on arduinos
    adc_init();

    // set up pwm for rgb pins
    pwm_init_d9d10();
    pwm_init_d11();

    // start timing for pwm
    uint32_t now = 0;

    // MAIN LOOP
    while (1) {           
        now++; // increment timing

        if (!getState(ui.pwrSw)) { // pwrSw off
            off(rgb); // turn off rgb
            allBits(sr, ui, 6, 0); // all bits off
            continue; // break loop
        }

        // turn on rgb if A0 on
        if (readA5()) {
            on(rgb);
            pulse(&rgb, now, 1, adc_rgb_pot(4));
        } else {
            off(rgb);   
        }
        
        // run bitchaser if second switch is on, else all leds on
        if (getState(ui.seqSw)) { 
            if (getState(ui.intnSw)) { // check if intensity switch on
                if (readA0()) {
                    revBitChaser(sr, ui, adc_sw(3));
                }
                bitChaser(sr, ui, adc_sw(3)); // changes divider with a3 pot
            } else {
                if (readA0()) {
                    revBitChaser(sr, ui, 6);
                } else {
                    bitChaser(sr, ui, 6); // single light on at a time
                }
                
            }
        // sequence off, all lights on
        } else {
            allBits(sr, ui, 6, 1); // all bits on
        }
    }
}