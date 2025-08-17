#include <avr/io.h> 
#include <util/delay.h>
#include "ui.h"

uint8_t getState(uint8_t pin) {
   return PIND & pin;    
}

uint8_t checkStates(struct usrIn ui, uint8_t lastStates[]) {
    static uint8_t curStates[6];
    curStates[0] = getState(ui.pwrSw);
    curStates[1] = getState(ui.seqSw);
    curStates[2] = getState(ui.intnSw);
    curStates[3] = getState(ui.altSw);
    curStates[4] = getState(ui.alt2Sw);
    curStates[5] = adc_sw(3); // switch to control numsr in intn mode

    // handle state change for any switch/pot
    for (int i = 0; i < 6; i++) {
        if (curStates[i] != lastStates[i]) {
            // don't return for 
            if (i == 5 && getState(ui.seqSw) && !getState(ui.intnSw)) {
                continue;
            } 
            return 1; // return interrupt signal    
        }
    }
    return 0;
}

uint8_t* getStates(struct usrIn ui) {
    static uint8_t states[6];
    states[0] = getState(ui.pwrSw);
    states[1] = getState(ui.seqSw);
    states[2] = getState(ui.intnSw);
    states[3] = getState(ui.altSw);
    states[4] = getState(ui.alt2Sw);
    states[5] = adc_sw(3);
    return states;
}

// setup analog pins
void adc_init(void) {
    // Reference = AVcc (5V) with external capacitor at AREF
    ADMUX = (1 << REFS0);

    // Enable ADC, prescaler = 128 -> ADC clock = 16 MHz / 128 = 125 kHz
    ADCSRA = (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // (optional) disable digital input buffer on ADC1 to save power
    DIDR0 |= (1 << ADC1D);
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
    if (val < (255 / 4)) {
        return 1;
    } else if (val < (255 / 2)) {
        return 2;
    } else {
        return 3;
    }
}

// set up pwm pins
void pwm_init(uint8_t pin) {
    DDRD |= pin;

    // Fast PWM, non-inverting, 8-bit
    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
    TCCR0B = (1 << CS01);  // prescaler = 8
}