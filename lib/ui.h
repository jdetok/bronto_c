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

typedef struct {
    uint8_t brt; // shift reg led brightness
    uint8_t spd; // delay speed
    uint8_t idv; // intn mode bitChaser divider
    uint8_t rgbrt; // rgb brightness
} pot_states;

typedef struct {
    uint8_t brtPot;
    uint8_t spdPot;
    uint8_t idvPot;
    uint8_t rgbrtPot;
} pots;

void switch_init(switches *sw);
uint8_t getState(uint8_t pin, char reg);
void getSwStates(switches *sw);
uint8_t stateSum(switches *sw);
uint8_t updateStates(switches *sw);
#endif