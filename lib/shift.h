#ifndef SHIFT_H
#define SHIFT_H

#include <stdint.h>

extern const uint16_t BIT16;
extern const uint16_t DTIME;

// struct for shift register
struct shiftReg {
    uint8_t ser;
    uint8_t oe;
    uint8_t latch;
    uint8_t clock;
};

// struct for progress lights
struct progLights {
    uint8_t blu;
    uint8_t wht;
    uint8_t pnk;
    uint8_t grn;
};

void delay_ms_var(uint8_t ms);
void bit16Chaser(struct shiftReg sr);
void bitChaser(struct shiftReg sr, int numSr);
void resetProg(struct progLights pl);
void first8(struct shiftReg sr);
void last8(struct shiftReg sr);
void shift8(struct shiftReg sr, uint8_t bits);
void shift16(struct shiftReg sr, uint16_t data);
void count16(struct shiftReg sr, struct progLights pl);

#endif