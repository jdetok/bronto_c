#ifndef UI_H
#define UI_H

#include <stdint.h>

struct usrIn {
    uint8_t intnSw; // d2 - intensity
    uint8_t seqSw; // d3 - solid on or sequence
    uint8_t pwrSw; // d5 - on off
    uint8_t altSw; // a0 (a7)
    uint8_t alt2Sw; // a5 (a7)
};

void adc_init(void);
void pwm_init_d6();
void pwm_init_d9d10();
uint8_t readA0();
uint8_t readA5();void pwm_init_d11();
uint16_t adc_read(uint8_t channel);
uint8_t adc_sw(uint8_t channel);
uint8_t adc_rgb_pot(uint8_t channel);
uint8_t getState(uint8_t pin);
uint8_t checkStates(struct usrIn ui, uint8_t lastStates[9]);
void getStates(struct usrIn ui, uint8_t states[9]);

#endif