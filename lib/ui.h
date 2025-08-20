#ifndef UI_H
#define UI_H

#include <stdint.h>
#include <avr/io.h> 
#include <util/delay.h>

typedef struct {
    uint8_t intn; // intensity mode (only in chaser)
    uint8_t seq; // sequence or solid
    uint8_t pwr; // power on off
    uint8_t rev; // chaser reverse
    uint8_t rgb; // rgb on off
} sw_states;

typedef struct {
    uint8_t intn_sw; // d2 - intensity
    uint8_t seq_sw; // d3 - solid on or sequence
    uint8_t pwr_sw; // d5 - on off
    uint8_t rev_sw; // a0 (a7)
    uint8_t rgb_sw; // a5 (a7)
    sw_states states;
} switches;

void pot_init();
void switch_init(switches *sw);
void current_states(switches *sw);
uint8_t get_state(uint8_t pin, char reg);
uint8_t sum_states(switches *sw);
uint8_t update_states(switches *sw);
uint16_t read_pot(uint8_t channel);
#endif