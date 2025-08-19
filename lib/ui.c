#include <avr/io.h> 
#include <util/delay.h>
#include "ui.h"

uint8_t getState(uint8_t pin) {
   return PIND & pin;    
}

uint8_t readA0() {
    return (PINC &(1 << PC0)) ? 1 : 0;
}

uint8_t readA5() {
    return (PINC &(1 << PC5)) ? 1 : 0;
}

// pass last states array & compare, return a 1 to kill current loop
uint8_t checkStates(struct usrIn ui, uint8_t lastStates[9]) {
    uint8_t states[9];
    getStates(ui, states);

    // handle state change for any switch/pot
    for (int i = 0; i < 9; i++) {
        if (states[i] != lastStates[i]) {
            // don't return for intn pot changes
            if ((i > 6) && getState(ui.seqSw) && !getState(ui.intnSw)) {
                continue;
            } 
            return 1; // return interrupt signal    
        }
    }
    return 0;
}
// pass the array instead so it doesn't have to be static
void getStates(struct usrIn ui, uint8_t states[9]) {
    // static uint8_t states[9];
    states[0] = getState(ui.pwrSw);
    states[1] = getState(ui.seqSw);
    states[2] = getState(ui.intnSw);
    states[3] = getState(ui.altSw);
    states[4] = getState(ui.alt2Sw);
    states[5] = readA0(); // rgb on off
    states[6] = readA5(); // rgb other switch
    states[7] = adc_sw(3); // switch to control numsr in intn mode
    states[8] = adc_sw(4); // switch to control rgb brightness
}

// setup analog pins
void adc_init(void) {
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
void pwm_init_d6() {
    DDRD |= 1 << PD6;

    // Fast PWM, non-inverting, 8-bit
    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
    TCCR0B = (1 << CS01);  // prescaler = 8
}

// control (r)gb with OCR1A
void pwm_init_d9d10() {
    DDRB |= (1 << PB1) | (1 << PB2);  // set D9 as output (OC1A)

    // Fast PWM 8-bit mode, non-inverting
    TCCR1A = (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);
    TCCR1B = (1 << WGM12) | (1 << CS11);  // prescaler = 8
}

// control rg(b) with OCR2A
void pwm_init_d11() {
    DDRB |= (1 << PB3);  // set D11 as output (OC2A)

    // Fast PWM, non-inverting, 8-bit
    TCCR2A = (1 << WGM20) | (1 << WGM21) | (1 << COM2A1);
    TCCR2B = (1 << CS21);  // prescaler = 8
}