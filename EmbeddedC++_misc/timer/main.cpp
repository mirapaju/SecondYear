#include <iostream>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/timer.h"

#define LED_PIN 21
#define INTERVAL 500

bool led_toggle(struct repeating_timer *t) {
    gpio_put(LED_PIN, !gpio_get(LED_PIN));
    return true;
}

int main() {
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    struct repeating_timer timer;
    add_repeating_timer_ms(INTERVAL, led_toggle, NULL, &timer);

    while (true) {
        tight_loop_contents();
        //cancel_repeating_timer(&timer);
    }
    return 0;
}
