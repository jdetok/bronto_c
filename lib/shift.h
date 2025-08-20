#ifndef SHIFT_H
#define SHIFT_H

#include <avr/io.h> 
#include <util/delay.h>
#include <stdint.h>
#include "ui.h"
// #include "analog.h"

// struct for shift register
typedef struct {
    uint8_t ser;
    uint8_t oe;
    uint8_t latch;
    uint8_t clock;
} shiftReg;

void srPulse(shiftReg *sr, uint8_t srPin);
void set_brt();
void del();
void oe_pwm();
void shift_init(shiftReg *sr);
void delay_ms_var(uint8_t ms);
void allBits(shiftReg *sr, switches *sw, int numSr, int on);  
void bitChaser(shiftReg *sr, switches *sw, int numSr, uint8_t rev);
uint8_t read_intn(uint8_t channel);

#endif