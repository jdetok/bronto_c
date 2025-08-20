#include <avr/io.h> 
#include <util/delay.h>
#include "analog.h"
#include "ui.h"

uint8_t getState(uint8_t pin, char reg) {
   if (reg == 'd') {
        return PIND & pin;    
   }
   if (reg == 'c') {
    return PINC & pin;
   }
}

// pass last states array & compare, return a 1 to kill current loop
uint8_t checkStates(struct usrIn ui, uint8_t lastStates[7]) {
    uint8_t states[7];
    getStates(ui, states);

    // handle state change for any switch/pot
    for (int i = 0; i < 7; i++) {
        if (states[i] != lastStates[i]) {
            // don't return for intn pot changes
            if ((i > 4) && getState(ui.seqSw, 'd') && !getState(ui.intnSw, 'd')) {
                continue;
            } 
            return 1; // return interrupt signal    
        }
    }
    return 0;
}
// pass the array instead so it doesn't have to be static
void getStates(struct usrIn ui, uint8_t states[7]) {
    // static uint8_t states[9];
    states[0] = getState(ui.pwrSw, 'd');
    states[1] = getState(ui.seqSw, 'd');
    states[2] = getState(ui.intnSw, 'd');
    states[3] = getState(ui.revSw, 'c');
    states[4] = getState(ui.rgbSw, 'c');
    states[5] = adc_sw(3); // switch to control numsr in intn mode
    states[6] = adc_sw(4); // switch to control rgb brightness
}

