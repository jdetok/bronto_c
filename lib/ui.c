#include "ui.h"

// setup digital pins (& analog A0 A5) for switches
void switch_init(switches *sw) {
    *sw = (switches){
        .switches = {
            {.pin = PWR_SW, .bit = PWR_BIT, .reg = 'd'},
            {.pin = MOD_SW, .bit = MOD_BIT, .reg = 'd'},
            {.pin = REV_SW, .bit = REV_BIT, .reg = 'd'},
            {.pin = DIV_SW, .bit = DIV_BIT, .reg = 'c'},
            {.pin = RGB_SW, .bit = RGB_BIT, .reg = 'c'},
        },
        .state = 0
    };
}

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

    for (switch_id i = 0; i < SW_COUNT; i++) {
        set_state(&sw->state, sw->switches[i].pin,
            sw->switches[i].reg,
            sw->switches[i].bit);
    }

    // set 1 2 or 3 from read_div_pot to bits 5 and 6
    uint8_t div = read_div_pot();
    sw->state &= ~DIV_POT_MASK;
    sw->state |= (div << DIV_POT_SHIFT);

    // TODO: RGB POT STATE

    return (sw->state != cur_state) ? 1 : 0;
}

uint8_t get_div_pot(uint8_t state) {
    return (state >> DIV_POT_SHIFT) & 0x03;
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

// passed as brt to rgb pulse() function (divide pin read by val)
uint8_t read_rgb_brt() {
    uint16_t val = read_pot(RGB_POT);
    if (val < 50) {
        return 2;
    } else if (val < 100) {
        return 8;
    } else if (val < 150) {
        return 12;
    } else if (val < 200) {
        return 24;
    } else {
        return 36;
    }
}
// read intensity switch, return value will be num_sr value in chaser
uint8_t read_div_pot() {
    uint16_t val = read_pot(DIV_POT);
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
    OCR0A = read_pot(BRT_POT); // set brightness
}