#include <avr/io.h> 
#include "rgb.h"
#include "analog.h"
 
// setup pwm for rgb | red d9 OCR1A | green d10 OCR1B| blue d11 OCR2A
void rgb_pwm(struct rgbLED *rgb) {
    // setup digital outputs
    DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3); // d9 d10 d11
    // DDRB |= ; // d11

    // Timer1 Fast PWM 8-bit, non-inverting
    TCCR1A = (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);
    TCCR1B = (1 << WGM12) | (1 << CS11); // prescaler = 8

    // Timer2 Fast PWM 8-bit, non-inverting
    TCCR2A = (1 << WGM20) | (1 << WGM21) | (1 << COM2A1);
    TCCR2B = (1 << CS21); // prescaler = 8

    // set startup vals
    rgb->r = 0; rgb->g = 0; rgb->b = 0;
    rgb->dir_r = 1; rgb->dir_g = 1; rgb->dir_b = 1;
    rgb->last_update = 0;
}

// setup analog pins
void adc_init() {
    // Reference = AVcc (5V) with external capacitor at AREF
    ADMUX = (1 << REFS0);

    // Enable ADC, prescaler = 128 -> ADC clock = 16 MHz / 128 = 125 kHz
    ADCSRA = (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

// read analog channel
uint16_t adc_read(uint8_t channel) {
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

uint8_t adc_sw(uint8_t channel) {
    uint16_t val = adc_read(channel);
    if (val < 50) {
        return 1;
    } else if (val < 150) {
        return 2;
    } else {
        return 3;
    }
}

// passed as brt to rgb pulse() function (divide pin read by val)
uint8_t adc_rgb_pot(uint8_t channel) {
    uint16_t val = adc_read(channel);
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

// control sr oe pin (all leds brightness) with OCR0A
void oe_pwm() {
    DDRD |= 1 << PD6;

    // Fast PWM, non-inverting, 8-bit
    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
    TCCR0B = (1 << CS01);  // prescaler = 8
}