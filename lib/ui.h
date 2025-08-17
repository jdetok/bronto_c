#ifndef UI_H
#define UI_H
#include <stdint.h>
struct usrIn {
    uint8_t pwrSw;
    uint8_t ptrnSw;
    uint8_t spdPot;
    uint8_t brtSw;
};
void adc_init(void);
uint16_t adc_read(uint8_t channel);
uint8_t adc_scaled(uint8_t channel);
void pwm_init(uint8_t pin);
#endif