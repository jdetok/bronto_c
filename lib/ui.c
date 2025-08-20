#include "ui.h"

// setup digital pins (& analog A0 A5) for switches
void switch_init(switches *sw) {
    sw->intn_sw = 1 << PD2;
    sw->seq_sw = 1 << PD3;
    sw->pwr_sw = 1 << PD5;
    sw->rev_sw = 1 << PC0;
    sw->rgb_sw = 1 << PC5;
}

// setup analog pins
void pot_init() {
    // Reference = AVcc (5V) with external capacitor at AREF
    ADMUX = (1 << REFS0);

    // Enable ADC, prescaler = 128 -> ADC clock = 16 MHz / 128 = 125 kHz
    ADCSRA = (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

// read analog channel
uint16_t read_pot(uint8_t channel) {
    // select channel (0–7), clear MUX bits first
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

    // start conversion
    ADCSRA |= (1 << ADSC);

    // wait for conversion to finish
    while (ADCSRA & (1 << ADSC));

    // don't return 255, will turn all LED off
    uint16_t val = ADC >> 2;
    if (val > 254) {
        return 254;
    }
    return val;  // 10-bit result (0–1023)
}

uint8_t get_state(uint8_t pin, char reg) {
   if (reg == 'd') {
        return PIND & pin;    
   }
   if (reg == 'c') {
    return PINC & pin;
   }
}

// TODO: new get_states() that accepts a pointer to a states struct & updates it
// 1770 bytes before implementing

void current_states(switches *sw) {
    sw->states.pwr = get_state(sw->pwr_sw, 'd');
    sw->states.seq = get_state(sw->seq_sw, 'd');
    sw->states.intn = get_state(sw->intn_sw, 'd');
    sw->states.rev = get_state(sw->rev_sw, 'c');
    sw->states.rgb = get_state(sw->rgb_sw, 'c');
}

// sum the states, use in update_states to determine whether there was a state change
uint8_t sum_states(switches *sw) {
    return (
        sw->states.pwr + sw->states.seq + sw->states.intn + sw->states.rev + sw->states.rgb);
}

uint8_t update_states(switches *sw) {
    // record state before update
    uint8_t stSum = sum_states(sw);

    // get current states
    current_states(sw);

    // check if state sum if different after update, return 1 if so
    if (stSum != sum_states(sw)) {
        return 1;
    }
    return 0;
}