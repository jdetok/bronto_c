#include <avr/io.h> 
#include <util/delay.h>
#include "../lib/shift.h"
#include "../lib/ui.h"

int main() {
    // SETUP
    struct shiftReg sr = { // SHIFT REGISTER PINS
        1 << PD4, // d4 serial pin
        1 << PD6, // d6 oe pin
        1 << PD7, // d7 latch pin
        1 << PB0, // d8 clock pin
    };
    struct usrIn ui = { // SWITCH/POTS PINS
        1 << PD2, // d2 intensity switch
        1 << PD3, // d3 on vs seq switch
        1 << PD5, // d5 power switch
        1 << PC0, // a0 alt switch
        1 << PC5, // a5 alt switch 2
    };

    adc_init(); // SETUP ANALOG REGISTERS
    pwm_init(sr.oe); // SETUP PULSE WIDTH REGISTERS
    
    // setup pins as output
    DDRD |= sr.ser | sr.oe | sr.latch; // d register output pins
    DDRB |= sr.clock; // b reg output pins
    DDRD &= ~ui.pwrSw | ~ui.seqSw | ~ui.intnSw; // d reg input pins
    DDRC &= ~ui.altSw | ~ui.alt2Sw; // c reg input pins

    // MAIN LOOP
    while (1) {
        // handle pwrSw off
        if (!getState(ui.pwrSw)) {
            allBits(sr, ui, 6, 0); // all bits off
            continue;
        }
        
        // check sequence switch
        if (PIND & ui.seqSw) { 
            if (PIND & ui.intnSw) { // check intensity switch
                bitChaser(sr, ui, adc_sw(3)); // changes with a3 pot
            } else {
                bitChaser(sr, ui, 6);
            }
        // sequence off, all lights on
        } else {
            allBits(sr, ui, 6, 1); // all bits on
        }
    }
}