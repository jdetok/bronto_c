#include "ui.h"

// state.c
// SET, UPDATE, & GET STATE IN sw.state BYTE
// state for all five switches & the top two (MOD_POT & RGB_POT) are stored
// as bitfields - all states are stored within 7 bits

// check if bit for pin is set in state
uint8_t switch_state(switches *sw, switch_id id) {
    if (id >= SW_COUNT) return 0;
    return (sw->state & sw->switches[id].bit) ? 1 : 0;          
}

void set_state(uint8_t *state, uint8_t pin, char reg, uint8_t bit) {
    uint8_t val = 0;
    
    // PINC register for c, PIND register for d
    if (reg == 'c') {
        val = PIN_STATE(PINC, pin);
    } else if (reg == 'd') {
        val = PIN_STATE(PIND, pin);
    }
    
    // update state with val
    if (val) {
        *state |= bit;
    } else {
        *state &= ~bit;
    }
}

uint8_t check_state(switches *sw) {
    uint8_t cur_state = sw->state;

    // loop through switch_id enum to check state
    for (switch_id i = 0; i < SW_COUNT; i++) {
        set_state(&sw->state, sw->switches[i].pin,
            sw->switches[i].reg,
            sw->switches[i].bit);
    }

    // set 1 2 or 3 from read_div_pot to bits 5 and 6
    uint8_t mod = read_mod_pot();
    sw->state &= ~MOD_POT_MASK;
    sw->state |= (mod << MOD_POT_SHIFT);

    // TODO: RGB POT STATE

    return (sw->state != cur_state) ? 1 : 0;
}

uint8_t get_mod_pot(uint8_t state) {
    return (state >> MOD_POT_SHIFT) & 0x03;
}