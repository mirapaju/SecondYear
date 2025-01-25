#ifndef EEPROM_LED_STATES_STORING_H
#define EEPROM_LED_STATES_STORING_H
#include <stdint-gcc.h>
#include <stdbool.h>
#include <pico/types.h>

typedef struct ledstate {
    uint8_t state;
    uint8_t not_state;
} ledstate;

void set_led_state(ledstate *ls, uint8_t value);
bool led_state_is_valid(ledstate *ls);
void init_button(uint8_t pin_nro);
void init_LED(uint pin_nro);
void print_state_and_seconds(uint64_t start_time_us, uint8_t ledstates);
void update_leds(uint8_t led_states);
bool button_pushed(const uint *pins, size_t num_pins, uint *pressed_button);

#endif //EEPROM_LED_STATES_STORING_H
