#include "shift.h"

// required to accept delay time as a variable from pot
// _delay_ms() only accepts a compile time constant
void delay_ms_var(uint8_t ms) {
    while (ms--) {
        _delay_us(500);
    }
}

// delay
void del() {
    delay_ms_var(read_pot(SPD_POT)); // delaytime ms
}

void shift_init(shiftReg *sr) {
    // set pins to the appropriate bit on the register
    sr->ser = SER;
    sr->oe = OE;
    sr->latch = LATCH;
    sr->clock = CLOCK;

    // setup pins as output on their appropriate register
    DDRD |= sr->ser | sr->oe | sr->latch;
    DDRB |= sr->clock; 

    oe_pwm(); // setup pwm channel for oe pin to control brightness
}



// 0 for clock, 1 for latch
void pulse_pin(shiftReg *sr, uint8_t clk_latch) {
    switch (clk_latch) {
        case 0: // clock pin
            PORTB |= sr->clock;
            PORTB &= ~sr->clock;
            break;
        case 1: // latch pin
            PORTD |= sr->latch;
            PORTD &= ~sr->latch;
            break;
    }
}

// turn off all lights
void onoff(shiftReg *sr, switches *sw, int num_sr, int on) {
    int bits = num_sr * 8;
    for (int i = 0; i < bits; i++) {
        // uint8_t interrupt = update_states(sw);
        uint8_t interrupt = check_state(sw);
        if (interrupt) {
            return;
        } else {
            set_brt();
            if (on) {
                PORTD |= sr->ser;
            } else {
                PORTD &= ~sr->ser;
            }
            pulse_pin(sr, 0); // pulse clock
        }
    }
    pulse_pin(sr, 1); // pulse latch
}

void chaser(shiftReg *sr, switches *sw, int num_sr, uint8_t rev) {
    uint8_t bits = num_sr * 8;
    uint8_t start, end, step;

    if (!rev) {
        start = 0;
        end = bits;
        step = 1;
    } else {
        start = (bits - 1);
        end = -1;
        step = -1;
    }
    
    for (uint8_t i = start; i != end; i += step) {
        uint8_t interrupt = check_state(sw);
        if (interrupt) {
            return; // return if state changed
        } else {
            set_brt(); // set brightness
            // loop forward
            for (int b = (bits - 1); b >= 0; b--) {
                if (b == i) { // when current bit position (b) is same as current led (i), send a 1 to serial pin
                    PORTD |= sr->ser; // write a 1
                } else {
                    PORTD &= ~sr->ser; // write a 0
                }
                pulse_pin(sr, 0); // shift clock
            }
        }
        pulse_pin(sr, 1); // shift latch
        del(); // delay
    }
}

void byte_chaser(shiftReg *sr, switches *sw, int num_sr) {
    uint8_t bits = num_sr * 8;
    // outer loop through number of shift registers
    for (int i = 0; i < num_sr; i++) {
        // check that switch states haven't changed, exit if it has
        uint8_t interrupt = check_state(sw);
        if (interrupt) {
            return;
        } else {
            set_brt(); // set brightness
            for (int b = bits - 1; b >= 0; b--) {
                if (b / 8 == i) {
                    PORTD |= sr->ser; // write a 1
                } else {
                    PORTD &= ~sr->ser; // write a 0
                }
                pulse_pin(sr, 0); // shift clock
            }
        }
        pulse_pin(sr, 1); // shift latchs
        del(); // delay
    }
}