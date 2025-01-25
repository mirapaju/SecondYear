#include <iostream>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/timer.h"
#include <queue>
#include "ssd1306.h"
#include <cstring>


#define NUM_LEDS 3
#define LED_MAX_STATES 7
#define LED_1 20
#define LED_2 21
#define LED_3 22

//set the toggling interval according to the led you want toggle the fastest
//20 -> 125ms -> 4/sec
//21 -> 250ms -> 2/sec
//22 -> 500ms -> 1/sec
#define INTERVAL 125

//set the printing interval according to how often you want to print out the time elapsed
#define PRINT_INTERVAL 1000
#define US_PER_S (1000000)

#define CHAR_LEN 8
#define HEIGHT 64
#define WIDTH 128

std::queue<int> events;

static const unsigned char binary_data[] = {
        // font edit begin : monovlsb : 39 : 39 : 39
        0x10, 0x08, 0x0C, 0x04, 0x06, 0xA3, 0xCF, 0x10,
        0x18, 0x0C, 0x04, 0x83, 0xF1, 0x18, 0x0C, 0x07,
        0x01, 0x00, 0xC0, 0x60, 0x30, 0x08, 0x04, 0x06,
        0x83, 0xC1, 0x31, 0x0F, 0x01, 0x00, 0x00, 0x80,
        0xC0, 0x40, 0x60, 0x30, 0x10, 0x18, 0x7C, 0x00,
        0xC0, 0x60, 0x38, 0x0E, 0x03, 0x00, 0x00, 0x80,
        0xE0, 0x3C, 0x07, 0x80, 0xE0, 0x30, 0x0C, 0x06,
        0x03, 0x00, 0xC0, 0x60, 0x18, 0x0C, 0x07, 0x81,
        0xC0, 0x60, 0x38, 0x0C, 0x06, 0x03, 0x01, 0x00,
        0x80, 0xC0, 0x30, 0x04, 0x01, 0x00, 0x03, 0x01,
        0x00, 0x00, 0x80, 0x60, 0x98, 0xCE, 0x63, 0x38,
        0x0C, 0x03, 0x01, 0x80, 0x60, 0x38, 0x0C, 0x06,
        0xC3, 0x61, 0x30, 0x18, 0x06, 0x03, 0x01, 0x00,
        0x00, 0x80, 0x40, 0xB0, 0xD8, 0x6C, 0x23, 0xB1,
        0xD0, 0x78, 0x38, 0x10, 0x80, 0xE0, 0xF8, 0x6C,
        0x37, 0x19, 0x06, 0x01, 0x00, 0x80, 0xC0, 0xB0,
        0xCC, 0x67, 0x11, 0x1C, 0x04, 0x02, 0x01, 0x00,
        0x00, 0x00, 0x80, 0xE0, 0x70, 0x1C, 0x1E, 0x0F,
        0x07, 0x03, 0xC1, 0x70, 0x1C, 0x86, 0x83, 0x80,
        0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,
        0x00, 0x70, 0x3C, 0x1E, 0x07, 0x03, 0x01, 0x00,
        0x00, 0x80, 0x40, 0x60, 0xE0, 0x00, 0x00, 0x0C,
        0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0,
        0xC7, 0x07, 0x03, 0x01, 0x01, 0x1E, 0x13, 0x50,
        0x70, 0x00, 0x00
        // font edit end
};

int get_elapsed_time(absolute_time_t start_time){
    absolute_time_t current_time = get_absolute_time();
    uint32_t elapsed_seconds = absolute_time_diff_us(start_time, current_time) / US_PER_S;
    printf("Elapsed time: %d seconds\n", elapsed_seconds);
    return static_cast<int>(elapsed_seconds);
}

void update_time(const char *str, ssd1306 display, int sec){
    char time_str[20];
    int16_t y = 0;
    int16_t str_len = strlen(str) * CHAR_LEN;

    mono_vlsb bfr(WIDTH - str_len, CHAR_LEN);
    sprintf(time_str, "%d", sec);
    display.blit(bfr, str_len, y);
    display.text(time_str, str_len, y);
    display.show();
}

bool tmr_cb(struct repeating_timer *t){
    events.push(1);
    return true;
}

int main() {
    stdio_init_all();
    printf("Boot\n");

    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(14, GPIO_FUNC_I2C);
    gpio_set_function(15, GPIO_FUNC_I2C);
    ssd1306 display(i2c1);

    char t_str[] = "TIME : ";
    display.fill(0);
    display.text(t_str, 0, 0);
    mono_vlsb rb(binary_data, 39, 39);
    display.blit(rb, 0, CHAR_LEN*2);
    display.show();

    absolute_time_t start_time = get_absolute_time();
    struct repeating_timer tmr;
    if (add_repeating_timer_ms(PRINT_INTERVAL, tmr_cb, NULL, &tmr)) printf("Timer added\n");

    const uint leds[NUM_LEDS] = {LED_1, LED_2, LED_3};
    for (auto pin : leds) {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);
    }

    int counter;
    int led;
    int seconds_elapsed;
    while (true) {
        if (!events.empty()){
            seconds_elapsed = get_elapsed_time(start_time);
            update_time(t_str, display, seconds_elapsed);
        }
        for (counter = 0; counter < LED_MAX_STATES; counter++) {
            for (led = 0; led < NUM_LEDS; led++) {
                gpio_put(leds[led], (counter >> led) & 1);
            }
            sleep_ms(INTERVAL);
        }
    }
    return 0;
}
