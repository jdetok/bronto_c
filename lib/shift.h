#ifndef SHIFT_H
#define SHIFT_H

#include <avr/io.h> 
#include <util/delay.h>
#include <stdint.h>
#include "ui/ui.h"

// update if circuit has a different number of shift registers
#define NUM_SR 6

// digital pins for shift register
#define SER (1 << PD4)
#define OE (1 << PD6)
#define LATCH (1 << PD7)
#define CLOCK (1 << PB0)

// struct to structure shift register pins together
typedef struct {
    uint8_t ser;
    uint8_t oe;
    uint8_t latch;
    uint8_t clock;
} shiftReg;

void shift_init(shiftReg *sr); // setup shift register struct
void delay_ms_var(uint8_t ms); // variable delay time
void set_brt(); // set shift register LED brightness
void del(); // calls delay_ms_var with variable delay from speed pot 
void pulse_pin(shiftReg *sr, uint8_t clk_latch); // set/clear clock or latch bit
void onoff(shiftReg *sr, switches *sw, int num_sr, int on); // all bits on/off
void chaser(shiftReg *sr, switches *sw, int num_sr, uint8_t rev); // single bit
void byte_chaser(shiftReg *sr, switches *sw, int num_sr, uint8_t rev); // byte
void mode_selector(shiftReg *sr, switches *sw, uint8_t mode); // call chaser

#endif