#include <avr/io.h> 
#include <util/delay.h>
#include "ui.h"

void adc_init(void) {
    // Reference = AVcc (5V) with external capacitor at AREF
    ADMUX = (1 << REFS0);

    // Enable ADC, prescaler = 128 → ADC clock = 16 MHz / 128 = 125 kHz
    ADCSRA = (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // (optional) disable digital input buffer on ADC1 to save power
    DIDR0 |= (1 << ADC1D);
}

uint16_t adc_read(uint8_t channel) {
    // select channel (0–7), clear MUX bits first
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

    // start conversion
    ADCSRA |= (1 << ADSC);

    // wait for conversion to finish
    while (ADCSRA & (1 << ADSC));

    return ADC >> 2;  // 10-bit result (0–1023)
}

uint8_t adc_scaled(uint8_t channel) {
    uint16_t raw = adc_read(channel);   // 0–1023
    return raw >> 2;                    // divide by 4 → 0–255
}

void pwm_init(uint8_t pin) {
    DDRD |= pin;

    // Fast PWM, non-inverting, 8-bit
    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
    TCCR0B = (1 << CS01);  // prescaler = 8

    // Start with 50% duty cycle
    OCR0A = 128; // 0-255 range
}