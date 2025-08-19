#ifndef RGB_H
#define RGB_H

#include <stdint.h>

struct rgbLED {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    int8_t dir_r; // fade direction (+1/-1)
    int8_t dir_g;
    int8_t dir_b;
    uint32_t last_update; // last time PWM was updated
};
void rgb_pwm(struct rgbLED *rgb);

void pulse(struct rgbLED *rgb, uint32_t time, uint32_t speed_ms, uint8_t brt);
void off(struct rgbLED rgb);
void on(struct rgbLED rgb);

#endif