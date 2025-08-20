#ifndef RGB_H
#define RGB_H

#include <stdint.h>
#include <avr/io.h> 
#include <util/delay.h>
#include "ui.h"

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    int8_t dir_r; // fade direction (+1/-1)
    int8_t dir_g;
    int8_t dir_b;
    uint32_t last_update; // last time PWM was updated
} rgbLED;

void rgb_pwm(rgbLED *rgb);
uint8_t read_rgb_brt(uint8_t channel);
void pulse(rgbLED *rgb, uint32_t time, uint32_t speed_ms, uint8_t brt);
void rgb_off();
void rgb_on();

#endif