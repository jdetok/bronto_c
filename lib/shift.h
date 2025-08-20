#ifndef SHIFT_H
#define SHIFT_H

#include <stdint.h>
#include "ui.h"
#include "analog.h"

extern const uint16_t BIT16;
extern const uint16_t DTIME;

// struct for shift register
struct shiftReg {
    uint8_t ser;
    uint8_t oe;
    uint8_t latch;
    uint8_t clock;
};

void delay_ms_var(uint8_t ms);
void allBits(struct shiftReg sr, struct usrIn ui, int numSr, int onOff); 
void bitChaser(struct shiftReg sr, struct usrIn ui, int numSr);
void revBitChaser(struct shiftReg sr, struct usrIn ui, int numSr);

#endif