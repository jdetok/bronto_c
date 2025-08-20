#include <avr/io.h> 
#include <util/delay.h>
#include "analog.h"
#include "ui.h"

void switch_init(switches *sw) {
    sw->intnSw = 1 << PD2;
    sw->seqSw = 1 << PD3;
    sw->pwrSw = 1 << PD5;
    sw->revSw = 1 << PC0;
    sw->rgbSw = 1 << PC5;
}

uint8_t getState(uint8_t pin, char reg) {
   if (reg == 'd') {
        return PIND & pin;    
   }
   if (reg == 'c') {
    return PINC & pin;
   }
}

// TODO: new getStates() that accepts a pointer to a states struct & updates it
// 1770 bytes before implementing

void getSwStates(switches *sw) {
    sw->states.pwr = getState(sw->pwrSw, 'd');
    sw->states.seq = getState(sw->seqSw, 'd');
    sw->states.intn = getState(sw->intnSw, 'd');
    sw->states.rev = getState(sw->revSw, 'c');
    sw->states.rgb = getState(sw->rgbSw, 'c');
}

// sum the states, use in updateStates to determine whether there was a state change
uint8_t stateSum(switches *sw) {
    return (
        sw->states.pwr + sw->states.seq + sw->states.intn + sw->states.rev + sw->states.rgb);
}

uint8_t updateStates(switches *sw) {
    // record state before update
    uint8_t stSum = stateSum(sw);

    // get current states
    getSwStates(sw);

    // check if state sum if different after update, return 1 if so
    if (stSum != stateSum(sw)) {
        return 1;
    }
    return 0;
}