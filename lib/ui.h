#ifndef UI_H
#define UI_H

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h> 
#include <util/delay.h>

typedef struct {
    uint8_t intnSw; // d2 - intensity
    uint8_t seqSw; // d3 - solid on or sequence
    uint8_t pwrSw; // d5 - on off
    uint8_t revSw; // a0 (a7)
    uint8_t rgbSw; // a5 (a7)
} usrIn;

typedef struct {
    uint8_t intn; // intensity mode (only in bitchaser)
    uint8_t seq; // sequence or solid
    uint8_t pwr; // power on off
    uint8_t rev; // bitchaser reverse
    uint8_t rgb; // rgb on off
} sw_states;

typedef struct {
    uint8_t intnSw; // d2 - intensity
    uint8_t seqSw; // d3 - solid on or sequence
    uint8_t pwrSw; // d5 - on off
    uint8_t revSw; // a0 (a7)
    uint8_t rgbSw; // a5 (a7)
    sw_states states;
} switches;

void adc_init();
void switch_init(switches *sw);
void getSwStates(switches *sw);
uint8_t getState(uint8_t pin, char reg);
uint8_t stateSum(switches *sw);
uint8_t updateStates(switches *sw);
uint16_t adc_read(uint8_t channel);
#endif