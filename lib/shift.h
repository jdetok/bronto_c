#ifndef SHIFT_H
#define SHIFT_H

#include <avr/io.h> 
#include <util/delay.h>
#include <stdint.h>
#include "ui.h"

typedef struct {
    uint8_t ser;
    uint8_t oe;
    uint8_t latch;
    uint8_t clock;
} shiftReg;

void pulse_pin(shiftReg *sr, uint8_t clk_latch);
void set_brt();
void del();
void oe_pwm();
void shift_init(shiftReg *sr);
void delay_ms_var(uint8_t ms);
void onoff(shiftReg *sr, switches *sw, int num_sr, int on);  
void chaser(shiftReg *sr, switches *sw, int num_sr, uint8_t rev);
void byte_chaser(shiftReg *sr, switches *sw, int num_sr);
uint8_t read_intn(uint8_t channel);

#endif