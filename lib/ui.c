#include <avr/io.h> 
#include <util/delay.h>
#include "analog.h"
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

