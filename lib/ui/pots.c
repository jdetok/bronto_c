#include "ui.h"

// pots.c
// PONTENTIOMETER READINGS FROM ANALOG PINS

// read analog channel
uint8_t read_pot(uint8_t channel) {
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
    } else if (val <= 20) {
        return 0;
    }
    return val;
}

// passed as brt to rgb pulse() function (divide pin read by val)
uint8_t read_rgb_brt() {
    uint16_t val = read_pot(RGB_POT);
    if (val < 20) {
        return 2;
    } else if (val < 200) {
        return 4;
    } else if (val < 200) {
        return 10;
    } else {
        return 30;
    }
}

// read intensity switch, return value will be num_sr value in chaser
uint8_t read_mod_pot() {
    uint16_t val = read_pot(MOD_POT);
    if (val < 50) {
        return 1;
    } else if (val < 150) {
        return 2;
    } else {
        return 3;
    }
}