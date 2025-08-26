#include "ui.h"

// ui.c
// INITALIZE UI COMPONENTS

// setup digital pins (& analog A0 A5) for switches
void switch_init(switches *sw) {
    *sw = (switches){
        .switches = {
            {.pin = PWR_PIN, .bit = PWR_BIT, .reg = 'd'},
            {.pin = SEQ_PIN, .bit = SEQ_BIT, .reg = 'd'},
            {.pin = REV_PIN, .bit = REV_BIT, .reg = 'd'},
            {.pin = MOD_PIN, .bit = MOD_BIT, .reg = 'c'},
            {.pin = RGB_PIN, .bit = RGB_BIT, .reg = 'c'},
        },
        .state = 0
    };
}

// setup analog pins
void pot_init() {
    // Reference = AVcc (5V) with external capacitor at AREF
    ADMUX = (1 << REFS0);

    // Enable ADC, prescaler = 128 -> ADC clock = 16 MHz / 128 = 125 kHz
    ADCSRA = (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

// control sr oe pin (all leds brightness) with OCR0A
void oe_pwm() {
    // Fast PWM, non-inverting, 8-bit
    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
    TCCR0B = (1 << CS01);  // prescaler = 8
}
