#include "shift.h"

void mode_selector(shiftReg *sr, switches *sw, uint8_t mode) {
    switch (mode) {
    case 1:
        chaser(sr, sw, 2, switch_state(sw, REV_SW));
        break;
    case 2:
        chaser(sr, sw, 3, switch_state(sw, REV_SW));
        break;
    case 3:
        byte_chaser(sr, sw, NUM_SR, switch_state(sw, REV_SW));        
        break;
    }
    
}

// required to accept delay time as a variable from pot
// _delay_ms() only accepts a compile time constant
void delay_ms_var(uint8_t ms) {
    while (ms--) {
        _delay_us(800);
    }
}

// delay
void del() {
    delay_ms_var(read_pot(SPD_POT)); // delaytime ms
}

// set brightness for leds as analog reading of pin A1 
void set_brt() {
    OCR0A = read_pot(BRT_POT); // set brightness
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

// optimized chaser func
void chaser(shiftReg *sr, switches *sw, int num_sr, uint8_t rev) {
    uint8_t bits = num_sr * 8;
    uint64_t bitmask = rev ? (1ULL << (bits - 1)) : 1ULL;
    for (uint64_t i = 0; i < bits; i++) {
        uint8_t interrupt = check_state(sw);
        if (interrupt) {
            return; // return if state changed
        } else {
            set_brt(); // set brightness
            for (int b = (bits - 1); b >= 0; b--) {
                if (bitmask & (1ULL << b)) {
                    PORTD |= sr->ser;
                } else {
                    PORTD &= ~sr->ser;
                }
                pulse_pin(sr, 0);
            }
        }
        pulse_pin(sr, 1);
        del();
        bitmask = rev ? (bitmask >> 1) : (bitmask << 1);
    } 
}

void byte_chaser(shiftReg *sr, switches *sw, int num_sr, uint8_t rev) {
    uint8_t bits = num_sr * 8;
    uint64_t bitmask = rev ? (0xFFULL << (bits - 8)) : 0xFFULL;
    
    // outer loop through number of shift registers
    for (int i = 0; i < num_sr; i++) {
        // check that switch states haven't changed, exit if it has
        uint8_t interrupt = check_state(sw);
        if (interrupt) {
            return;
        } else {
            set_brt(); // set brightness
            uint64_t temp = bitmask;
            for (int b = 0; b < bits; b++) {
                if (temp & 1ULL) {
                    PORTD |= sr->ser;  // write 1
                } else {
                    PORTD &= ~sr->ser; // write 0
                }
                pulse_pin(sr, 0);      // shift clock
                temp >>= 1;             // move next bit into LSB
            }
        }
        pulse_pin(sr, 1); // shift latchs
        del(); // delay
        bitmask = rev ? (bitmask >> 8ULL) : (bitmask << 8ULL);
        // bitmask <<= 8ULL;/
    }
}
/*
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
}*/