#include <avr/io.h> 
#include <util/delay.h>
#include "rgb.h"
// #include "ui.h"

void off(struct rgbLED rgb) {
    // disconnect from pwm
    TCCR1A &= ~((1 << COM1A1) | (1 << COM1B1)); // d9 d10
    TCCR2A &= ~(1 << COM2A1); // d11

    // pins low
    PORTB &= ~((1 << PB1) | (1 << PB2) | (1 << PB3)); 
}

// enable pwm
void on(struct rgbLED rgb) {
    // enable pwm
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1); // d9 d10
    TCCR2A |= (1 << COM2A1); // d11
}

// pulse rgb with pwm
void pulse(struct rgbLED *rgb, uint32_t time, uint32_t speed_ms, uint8_t brt) {
    if (time - rgb->last_update < speed_ms) return;
    rgb->last_update = time;

    // Update RGB values
    if (rgb->r + rgb->dir_r > 255 || 
        rgb->r + rgb->dir_r < 0) rgb->dir_r = -rgb->dir_r;
    if (rgb->g + rgb->dir_g > 255 || 
        rgb->g + rgb->dir_g < 0) rgb->dir_g = -rgb->dir_g;
    if (rgb->b + rgb->dir_b > 255 || 
        rgb->b + rgb->dir_b < 0) rgb->dir_b = -rgb->dir_b;

    rgb->r += rgb->dir_r;
    rgb->g += rgb->dir_g;
    rgb->b += rgb->dir_b;

    // set brightness
    OCR1A = (rgb->r) / brt; // D9
    OCR1B = (rgb->g) / brt; // D10
    OCR2A = (rgb->b) / brt; // D11
}

