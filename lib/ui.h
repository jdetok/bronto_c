#ifndef UI_H
#define UI_H

#include <stdint.h>

typedef struct {
    uint8_t intnSw; // d2 - intensity
    uint8_t seqSw; // d3 - solid on or sequence
    uint8_t pwrSw; // d5 - on off
    uint8_t revSw; // a0 (a7)
    uint8_t rgbSw; // a5 (a7)
} usrIn;

uint8_t getState(uint8_t pin, char reg);
uint8_t checkStates(usrIn *ui, uint8_t lastStates[7]);
void getStates(usrIn *ui, uint8_t states[7]);

#endif