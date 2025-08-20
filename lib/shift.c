#include <avr/io.h> 
#include <util/delay.h>
#include "shift.h"
#include "ui.h"

// required to accept delay time as a variable from pot
// _delay_ms() only accepts a compile time constant
void delay_ms_var(uint8_t ms) {
    while (ms--) {
        _delay_ms(1);
    }
}

// turn off all lights
void allBits(struct shiftReg sr, struct usrIn ui, int numSr, int on) {
    uint8_t states[7];
    int numBits = numSr * 8;
    getStates(ui, states);
    for (int i = 0; i < numBits; i++) {
        uint8_t interrupt = checkStates(ui, states);
        if (interrupt) {
            return;
        } else {
            // all bits on
            OCR0A = adc_read(1); // set brightness
            if (on) {
                PORTD |= sr.ser;
            } else {
                PORTD &= ~sr.ser;
            }
            PORTB |= sr.clock;
            PORTB &= ~sr.clock;
        }
    }
    PORTD |= sr.latch;
    PORTD &= ~sr.latch;
}

// TODO: new bitchaser accept readA0 output as param forward reverse
void bitChaser(struct shiftReg sr, struct usrIn ui, int numSr, uint8_t rev) {
    // current switch & pot states
    uint8_t states[7];
    getStates(ui, states);
    int numBits = numSr * 8;
    if (!rev) {
        // outer loop through number of LEDs
        for (int i = 0; i < numBits; i++) {
            // check that switch states haven't changed, exit if it has
            uint8_t interrupt = checkStates(ui, states);
            if (interrupt) {
                return;
            } else {
                // set brightness for each bit
                OCR0A = adc_read(1); // set brightness
                // loop forward
                for (int b = (numBits - 1); b >= 0; b--) { // inner loop through each bit
                    if (b == i) { // when current bit position (b) is same as current led (i), send a 1 to serial pin
                        PORTD |= sr.ser; // write a 1
                    } else {
                        PORTD &= ~sr.ser; // write a 0
                    }
                    // pulse clock (load individual bit to shift register)
                    PORTB |= sr.clock;
                    PORTB &= ~sr.clock;
                }
            }
            // pulse latch (load all bits in shift register to memory)
            PORTD |= sr.latch;
            PORTD &= ~sr.latch;
            delay_ms_var(adc_read(2)); // delaytime ms
        }
    } else { // REVERSE
        for (int i = numBits; i >= 0; i--) {
        // read current states & return if change detected
            uint8_t interrupt = checkStates(ui, states);
            if (interrupt) {
                return;
            } else {
                OCR0A = adc_read(1); // set brightness
                for (int b = (numBits - 1); b > 0; b--) { // inner loop through each bit
                    if (b == i) { // when current bit position (b) is same as current led (i), send a 1 to serial pin
                        PORTD |= sr.ser; // write a 1
                    } else {
                        PORTD &= ~sr.ser; // write a 0
                    }
                    // pulse clock (load individual bit to shift register)
                    PORTB |= sr.clock;
                    PORTB &= ~sr.clock;
                }
            // pulse latch (load all bits in shift register to memory)
            }
                // pulse latch (load all bits in shift register to memory)
            PORTD |= sr.latch;
            PORTD &= ~sr.latch;
            delay_ms_var(adc_read(2)); // delaytime ms
        }
    }
}