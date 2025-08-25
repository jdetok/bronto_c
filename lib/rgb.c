#include "rgb.h"

// TODO: PULSE PINK GREEN BLUE

// setup pwm for rgb | red d9 OCR1A | green d10 OCR1B| blue d11 OCR2A
void rgb_pwm(rgbLED *rgb) {
    // setup digital outputs
    DDRB |= RPIN | GPIN | BPIN; // d9 d10 d11

    // Timer1 Fast PWM 8-bit, non-inverting
    TCCR1A = (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);
    TCCR1B = (1 << WGM12) | (1 << CS11); // prescaler = 8

    // Timer2 Fast PWM 8-bit, non-inverting
    TCCR2A = (1 << WGM20) | (1 << WGM21) | (1 << COM2A1);
    TCCR2B = (1 << CS21); // prescaler = 8

    // set startup vals
    rgb->r = 0; rgb->g = 0; rgb->b = 0;
    rgb->dir_r = 1; rgb->dir_g = 1; rgb->dir_b = 1;
    rgb->last_update = 0;
}

void rgb_off() {
    // disconnect from pwm
    TCCR1A &= ~((1 << COM1A1) | (1 << COM1B1)); // d9 d10
    TCCR2A &= ~(1 << COM2A1); // d11

    // pins low
    PORTB &= ~(RPIN | GPIN | BPIN); 
}

// enable pwm for digital pins d9/10/11 for the RGB LED
void rgb_on() {
    TCCR1A |= RED_PWM | GRN_PWM; // d9 d10
    TCCR2A |= BLU_PWM; // d11
}

// pulse rgb with pwm
void pulse(rgbLED *rgb, uint32_t time, uint32_t speed_ms, uint8_t brt) {
    if (time - rgb->last_update < speed_ms) return;
    rgb->last_update = time;
    brt /= 2;
    // Update RGB values
    if (rgb->r + rgb->dir_r > brt || rgb->r + rgb->dir_r < 0) {
        rgb->dir_r = -rgb->dir_r;
    }
    if (rgb->g + rgb->dir_g > brt || rgb->g + rgb->dir_g < 0) {
        rgb->dir_g = -rgb->dir_g;
    } 
    if (rgb->b + rgb->dir_b > brt || rgb->b + rgb->dir_b < 0) {
        rgb->dir_b = -rgb->dir_b;
    } 

    rgb->r += rgb->dir_r;
    // rgb->g += rgb->dir_g;
    rgb->b += rgb->dir_b;
    TCCR1A &= ~(GRN_PWM);
    // TCCR1A &= ~(RED_PWM);


    // set brightness
    OCR1A = (rgb->r) / 1.5;// / brt; // D9
    // OCR1B = (rgb->g);// brt; // D10
    OCR2A = (rgb->b) / 1.5;// / brt; // D11
}

// // pulse rgb with pwm
// void pulse(rgbLED *rgb, uint32_t time, uint32_t speed_ms, uint8_t brt) {
//     if (time - rgb->last_update < speed_ms) return;
//     rgb->last_update = time;

//     // Update RGB values
//     if (rgb->r + rgb->dir_r > 255 || 
//         rgb->r + rgb->dir_r < 0) rgb->dir_r = -rgb->dir_r;
//     if (rgb->g + rgb->dir_g > 255 || 
//         rgb->g + rgb->dir_g < 0) rgb->dir_g = -rgb->dir_g;
//     if (rgb->b + rgb->dir_b > 255 || 
//         rgb->b + rgb->dir_b < 0) rgb->dir_b = -rgb->dir_b;

//     rgb->r += rgb->dir_r;
//     rgb->g += rgb->dir_g;
//     rgb->b += rgb->dir_b;

//     // set brightness
//     OCR1A = (rgb->r) / brt; // D9
//     OCR1B = (rgb->g) / brt; // D10
//     OCR2A = (rgb->b) / brt; // D11
// }