#include <avr/io.h> 
#include <util/delay.h>
// #include <stdio.h>
#include "../lib/shift.h"
#include "../lib/ui.h"
#include "../lib/rgb.h"
// #include "../lib/uart.h"


int main() {
    // digital pins for switches
    switches sw;
    switch_init(&sw);

    // setup digital pins (d4, d6, d7, d8) for shift register
    shiftReg sr;
    shift_init(&sr);

    // setup digital rgb pins - red d9 OCR1A | green d10 OCR1B| blue d11 OCR2A
    rgbLED rgb;
    rgb_pwm(&rgb);

    // setup analog pins (ADC0 - ADC5) A0-A5 on arduinos
    pot_init();

    // start timing for pwm
    uint32_t now = 0;

    // MAIN LOOP
    while (1) {           
        now++; // increment timing

        if (!get_state(sw.pwr_sw, 'd')) { // pwr_sw off
            rgb_off();
            onoff(&sr, &sw, 6, 0); // all bits off
            continue; // break loop
        }

        // turn on rgb if A5 on
        if (get_state(sw.rgb_sw, 'c')) {
            rgb_on();
            pulse(&rgb, now, 1, read_rgb_brt(4));
        } else {
            rgb_off();   
        }
        
        // run chaser if second switch is on, else all leds on
        if (get_state(sw.seq_sw, 'd')) { 
            if (get_state(sw.intn_sw, 'd')) { // check if intensity switch on
                // byte_chaser(&sr, &sw, 6);
               chaser(&sr, &sw, read_intn(3), get_state(sw.rev_sw, 'c'));
            } else {
                chaser(&sr, &sw, 6, get_state(sw.rev_sw, 'c'));
            }
        // sequence off, all lights on
        } else {
            onoff(&sr, &sw, 6, 1); // all bits on
        }
    }
}
