#ifndef RGB_H
#define RGB_H

#include <stdint.h>
#include <avr/io.h> 
#include <util/delay.h>

// digital pins for rgb
#define RPIN (1 << PB1)
#define GPIN (1 << PB2)
#define BPIN (1 << PB3)

// pwm enables
#define RED_PWM (1 << COM1A1)
#define GRN_PWM (1 << COM1B1)
#define BLU_PWM (1 << COM2A1)

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
void pulse(rgbLED *rgb, uint32_t time, uint32_t speed_ms, uint8_t brt);
void rgb_off();
void rgb_on();

#endif