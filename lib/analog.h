#ifndef ANALOG_H
#define ANALOG_H

#include <stdint.h>
#include "rgb.h"

void oe_pwm();
void rgb_pwm(struct rgbLED *rgb);
void adc_init(void);
uint16_t adc_read(uint8_t channel);
uint8_t adc_sw(uint8_t channel);
uint8_t adc_rgb_pot(uint8_t channel);

#endif