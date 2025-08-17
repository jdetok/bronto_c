#include <avr/io.h> 
#include <util/delay.h>
#include "shift.h"
#include "ui.h"

const uint16_t BIT16 = 0xFFFF; // 16 bit max size
const uint16_t DTIME = 500; // ms

void delay_ms_var(uint8_t ms) {
    while (ms--) {
        _delay_ms(1);
    }
}

// shift in one bit at a time, any number
void bitChaser(struct shiftReg sr, int numSr) {
    int numBits = numSr * 8;

    // outer loop through number of LEDs
    for (int i = 0; i < numBits; i++) {
        OCR0A = adc_read(2);

        for (int b = (numBits - 1); b >= 0; b--) {
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
        delay_ms_var(adc_read(1)); // delaytime ms
    }
}

// shift in one bit at a time, 16 bits
void bit16Chaser(struct shiftReg sr) {
    // outer loop through number of LEDs
    for (int i = 0; i < 16; i++) {
        // current bit mask, eliminates need for steps array
        uint16_t mask = 1 << i; 
        // process the bits
        for (int b=15; b>=0; b--) {
            // determine if in first or second 8bits
            if (mask & (1 << b)) { // compare bit in mask to current bit
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
        _delay_ms(100); // delaytime ms
    }
}

// half on half off
void first8(struct shiftReg sr) {
    for (int i=15; i>=0; i--) {
        // determine if in first or second 8bits
        if (i < 8) {
            uint8_t bits = 0xFF;
            PORTD |= sr.ser;
        } else {
            PORTD &= ~sr.ser;
        }
        PORTB |= sr.clock;
        PORTB &= ~sr.clock;
    }   
    PORTD |= sr.latch;
    PORTD &= ~sr.latch;
}

// half on half off
void last8(struct shiftReg sr) {
    for (int i=15; i>=0; i--) {
        // determine if in first or second 8bits
        if (i >= 8) {
            uint8_t bits = 0xFF;
            PORTD |= sr.ser;
        } else {
            PORTD &= ~sr.ser;
        }
        PORTB |= sr.clock;
        PORTB &= ~sr.clock;
    }   
    PORTD |= sr.latch;
    PORTD &= ~sr.latch;
}

void shift8(struct shiftReg sr, uint8_t bits) {
    
    for (int i=7; i>=0; i--) {
        if (bits & (1 << i)) {
            PORTD |= sr.ser;
        } else {
            PORTD &= ~sr.ser;
        }

        PORTB |= sr.clock;
        PORTB &= ~sr.clock;
    }   
    PORTD |= sr.latch;
    PORTD &= ~sr.latch;
}

// shift 16 bits of data into shift register
void shift16(struct shiftReg sr, uint16_t data) {
    for (int i = 15; i >= 0; i--) { // loop through 16 bits
        // write bit to ser
        if (data & (1 << i)) { // set current bit
            PORTD |= sr.ser; // ser high for current bit
        } else {
            PORTD &= ~sr.ser; // ser low for other bits
        }

        // pulse clock pin
        PORTB |= sr.clock; // srclk high
        PORTB &= ~sr.clock; // srclk low
    }

    // shift bits into memory
    PORTD |= sr.latch; // latch pin high
    PORTD &= ~sr.latch; // latch pin low
}

void count16(struct shiftReg sr, struct progLights pl) {
    for (uint16_t i=1; i<BIT16; i++) { // display count from 1 to 65535
        /* progress light
        less than 25% blue on, 25-50 blu wht, 50-75 blu wht pnk, 75-100 all
        */
        PORTB |= pl.blu; // blue always on - 0 - size/4
        if (i > (BIT16 / 4)) {
            PORTB |= pl.wht; // white on when > size/4
            if (i > (BIT16 / 2)) {
                PORTB |= pl.pnk; // pink on when > half size
            }
            if (i > (BIT16 / 4) * 3) {
                PORTB |= pl.grn; // green on when > 3/4 size
            }
        }
        // update bits in shift register
        shift16(sr, i); // shift in current bits
        // _delay_us(50); // .5 ms 
        _delay_ms(DTIME); // delaytime ms
    }
    // reset progress lights 
    resetProg(pl);   
}

// progress lights - used with counter
void resetProg(struct progLights pl) {
    PORTB &= ~pl.blu;
    PORTB &= ~pl.wht;
    PORTB &= ~pl.pnk;
    PORTB &= ~pl.grn;
}
