#ifndef SHIFT_H
#define SHIFT_H

#include <stdint.h>
#include "ui.h"
#include "analog.h"

extern const uint16_t BIT16;
extern const uint16_t DTIME;

// struct for shift register
typedef struct {
    uint8_t ser;
    uint8_t oe;
    uint8_t latch;
    uint8_t clock;
} shiftReg;

void delay_ms_var(uint8_t ms);
void allBits(shiftReg sr, usrIn ui, int numSr, int on); 
void bitChaser(shiftReg sr, usrIn ui, int numSr, uint8_t rev);

#endif