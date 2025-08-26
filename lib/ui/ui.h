#ifndef UI_H
#define UI_H

#include <stdint.h>
#include <avr/io.h> 
#include <util/delay.h>

// digital pins for switches
#define PWR_PIN (1 << PD5) // digital pin 5
#define SEQ_PIN (1 << PD3) // digital pin 3
#define REV_PIN (1 << PD2) // digital pin 2
#define MOD_PIN (1 << PC0) // analog pin 0 (using digital)
#define RGB_PIN (1 << PC5) // analog pin 5 (using digital)

// ADC channels correlating with analog pins
#define BRT_POT 1
#define SPD_POT 2
#define MOD_POT 3
#define RGB_POT 4

// return digital pin states for D and C reg
#define DPIN_STATE(P) ((PIND & (P)) ? 1 : 0)
#define CPIN_STATE(P) ((PINC & (P)) ? 1 : 0)
#define PIN_STATE(REG, PIN) (((REG) & (PIN)) ? 1 : 0)

// bit fields for switch states
#define PWR_BIT 0b1
#define SEQ_BIT 0b10
#define REV_BIT 0b100
#define MOD_BIT 0b1000
#define RGB_BIT 0b10000

// store div pot state in bits 5 & 6 (binary 3 left shifited 5 bits)
#define MOD_POT_SHIFT 5 
#define MOD_POT_MASK (0b11 << MOD_POT_SHIFT)

// enum for switches 
typedef enum {
    PWR_SW,
    SEQ_SW,
    REV_SW,
    MOD_SW,
    RGB_SW,
    SW_COUNT
} switch_id;

// struct holds a PIN, BIT, and char either 'c' or 'd' to denote register
typedef struct {
    uint8_t pin;
    uint8_t bit;
    char reg;
} switch_two_pos;

// struct to hold swithes - holds array of switches & state byte
typedef struct {
    switch_two_pos switches[5];
    uint8_t state; // use bitfields for state
} switches;

void pot_init(); // setup potentiometers to utilize PWM
void oe_pwm(); // setup PWM channels
void switch_init(switches *sw); // setup switches
void set_state(uint8_t *state, uint8_t pin, char reg, uint8_t bit); // set bit
uint8_t check_state(switches *sw);
uint8_t switch_state(switches *sw, switch_id id);
uint8_t read_pot(uint8_t channel); // returns 0 - 244
uint8_t read_mod_pot(); // writes 1-3 to bits 5-6 in state byte
uint8_t get_mod_pot(uint8_t state); // gets bits 5-6 for mode_selector (shift.h)
uint8_t read_rgb_brt(); // returns brightness divider for rgb pulse func

#endif