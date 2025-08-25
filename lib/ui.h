#ifndef UI_H
#define UI_H

#include <stdint.h>
#include <avr/io.h> 
#include <util/delay.h>

// digital pins for switches
#define PWR_SW (1 << PD5) // digital pin 5
#define MOD_SW (1 << PD3) // digital pin 3
#define REV_SW (1 << PD2) // digital pin 2
#define DIV_SW (1 << PC0) // analog pin 0 (using digital)
#define RGB_SW (1 << PC5) // analog pin 5 (using digital)

// ADC channels correlating with analog pins
#define BRT_POT 1
#define SPD_POT 2
#define DIV_POT 3
#define RGB_POT 4

#define DIV_POT_SHIFT 5 // state bits 5 & 6
#define DIV_POT_MASK (0b11 << DIV_POT_SHIFT)

// return digital pin states for D and C reg
#define DPIN_STATE(P) ((PIND & (P)) ? 1 : 0)
#define CPIN_STATE(P) ((PINC & (P)) ? 1 : 0)
#define PIN_STATE(REG, PIN) (((REG) & (PIN)) ? 1 : 0)

// bit fields for switch state
#define PWR_BIT 0b1
#define MOD_BIT 0b10
#define REV_BIT 0b100
#define DIV_BIT 0b1000
#define RGB_BIT 0b10000

typedef enum {
    SW_PWR = 0,
    SW_MOD,
    SW_REV,
    SW_DIV,
    SW_RGB,
    SW_COUNT
} switch_id;

typedef struct {
    uint8_t pin;
    uint8_t bit;
    char reg;
} switch_two_pos;

typedef struct {
    switch_two_pos switches[5];
    uint8_t state; // use bitfields for state
} switches;

void pot_init();
void oe_pwm();
void set_brt();
uint8_t get_div_pot(uint8_t state);
uint8_t read_div_pot();
// switches* switch_init();
void switch_init(switches *sw);
void set_state(uint8_t *state, uint8_t pin, char reg, uint8_t bit);
uint8_t check_state(switches *sw);
uint8_t switch_state(switches *sw, switch_id id);
uint16_t read_pot(uint8_t channel);
uint8_t read_rgb_brt();
#endif