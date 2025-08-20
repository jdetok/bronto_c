#include "shift.h"

// required to accept delay time as a variable from pot
// _delay_ms() only accepts a compile time constant
void delay_ms_var(uint8_t ms) {
    while (ms--) {
        _delay_ms(1);
    }
}

// delay
void del() {
    delay_ms_var(adc_read(2)); // delaytime ms
}

void shift_init(shiftReg *sr) {
    // set pins to the appropriate bit on the register
    sr->ser = 1 << PD4;
    sr->oe = 1 << PD6;
    sr->latch = 1 << PD7;
    sr->clock = 1 << PB0;

    // setup pins as output on their appropriate register
    DDRD |= sr->ser | sr->oe | sr->latch;
    DDRB |= sr->clock; 

    oe_pwm(); // setup pwm channel for oe pin to control brightness
}

// read intensity switch, return value will be numSr value in bitChaser
uint8_t read_intn(uint8_t channel) {
    uint16_t val = adc_read(channel);
    if (val < 50) {
        return 1;
    } else if (val < 150) {
        return 2;
    } else {
        return 3;
    }
}

// control sr oe pin (all leds brightness) with OCR0A
void oe_pwm() {
    // Fast PWM, non-inverting, 8-bit
    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
    TCCR0B = (1 << CS01);  // prescaler = 8
}

// set brightness for leds as analog reading of pin A1 
void set_brt() {
    OCR0A = adc_read(1); // set brightness
}

// 0 for clock, 1 for latch
void srPulse(shiftReg *sr, uint8_t srPin) {
    switch (srPin) {
        case 0:
            PORTB |= sr->clock;
            PORTB &= ~sr->clock;
            break;
        case 1:
            PORTD |= sr->latch;
            PORTD &= ~sr->latch;
            break;
    }
}

// turn off all lights
void allBits(shiftReg *sr, switches *sw, int numSr, int on) {
    int numBits = numSr * 8;
    for (int i = 0; i < numBits; i++) {
        uint8_t interrupt = updateStates(sw);
        if (interrupt) {
            return;
        } else {
            set_brt();
            if (on) {
                PORTD |= sr->ser;
            } else {
                PORTD &= ~sr->ser;
            }
            srPulse(sr, 0); // pulse clock
        }
    }
    srPulse(sr, 1); // pulse latch
}

// TODO: new bitchaser accept readA0 output as param forward reverse
void bitChaser(shiftReg *sr, switches *sw, int numSr, uint8_t rev) {
    int numBits = numSr * 8;
    if (!rev) {
        // outer loop through number of LEDs
        for (int i = 0; i < numBits; i++) {
            // check that switch states haven't changed, exit if it has
            uint8_t interrupt = updateStates(sw);
            if (interrupt) {
                return;
            } else {
                set_brt(); // set brightness
                // loop forward
                for (int b = (numBits - 1); b >= 0; b--) {
                    if (b == i) { // when current bit position (b) is same as current led (i), send a 1 to serial pin
                        PORTD |= sr->ser; // write a 1
                    } else {
                        PORTD &= ~sr->ser; // write a 0
                    }
                    srPulse(sr, 0); // shift clock
                }
            }
            srPulse(sr, 1); // shift latchs
            del(); // delay
        }
    } else { // REVERSE
        for (int i = numBits; i >= 0; i--) {
        // read current states & return if change detected
            uint8_t interrupt = updateStates(sw);
            if (interrupt) {
                return;
            } else {
                set_brt(); // set brightness
                for (int b = (numBits - 1); b > 0; b--) {
                    if (b == i) { // when current bit position (b) is same as current led (i), send a 1 to serial pin
                        PORTD |= sr->ser; // write a 1
                    } else {
                        PORTD &= ~sr->ser; // write a 0
                    }
                    srPulse(sr, 0); // pulse clock
                }
            }
            srPulse(sr, 1); // pulse latch
            del(); // delay
        }
    }
}