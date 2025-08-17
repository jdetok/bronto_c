#include <avr/io.h> 
#include <util/delay.h>
#include "../lib/shift.h"
#include "../lib/ui.h"



int main() {
    // initialize pins
    struct shiftReg sr = {1 << PD4, 1 << PD6, 1 << PD7, 1 << PB0};
    struct progLights pl = {1 << PB1, 1 << PB2, 1 << PB3, 1 << PB4};
    struct usrIn ui = {1 << PD5, 1 << PD3, 1 << PC1, };

    // setup pins as output
    DDRD |= sr.ser | sr.oe | sr.latch; // d register output pins
    DDRB |= sr.clock | pl.blu | pl.wht | pl.pnk | pl.grn; // b reg output pins
    DDRD &= ~ui.pwrSw | ~ui.ptrnSw; // power button input pin
    // DDRC &= ~ui.spdPot;

    adc_init();
    pwm_init(sr.oe);
    // main loop 
    while (1) {
        // uint8_t spd = adc_read(1);

        // check power switch
        if (PIND & ui.pwrSw) {
            PORTD &= ~sr.oe; 
        } else {
            PORTD |= sr.oe;
        }
        OCR0A = adc_scaled(2);
        // check pattern switch (really brightness)
        if (PIND & ui.ptrnSw) {
            
            bitChaser(sr, 3);
        } else {
            // OCR0A = 240;
            bitChaser(sr, 6);
        }
    }
}