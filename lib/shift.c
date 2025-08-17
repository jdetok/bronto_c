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
    int numBits = numSr * 8;
    uint8_t* states = getStates(ui);
    for (int i = 0; i < numBits; i++) {
        // EXIT IF STATE CHANGE DETECTED
        uint8_t interrupt = checkStates(ui, states);
        if (interrupt) {
            return;
        } else {
            // ALL BITS ON IF on > 0, ALL OFF OTHERWISE
            OCR0A = adc_read(1); // set brightness
            if (on) {
                PORTD |= sr.ser; // write a 1
            } else {
                PORTD &= ~sr.ser; // write a 0
            }

            // PULSE CLOCK, WRITE BIT TO SHIFT REGISTER
            PORTB |= sr.clock;
            PORTB &= ~sr.clock;
        }
        _delay_ms(1);
    }
    // WRITE ALL BITS TO MEMORY
    PORTD |= sr.latch;
    PORTD &= ~sr.latch;
}

// shift in one bit at a time, any number
void bitChaser(struct shiftReg sr, struct usrIn ui, int numSr) {
    // switch states at start of func
    uint8_t* states = getStates(ui);
    int numBits = numSr * 8;

    // outer loop through number of LEDs
    for (int i = 0; i < numBits; i++) {
        // read current states & return if change detected
        uint8_t interrupt = checkStates(ui, states);
        if (interrupt) {
            return;
        } else {
            OCR0A = adc_read(1); // set brightness
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
        // pulse latch (load all bits in shift register to memory)
        PORTD |= sr.latch;
        PORTD &= ~sr.latch;
        delay_ms_var(adc_read(2)); // delaytime ms
        }
    }
}