#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/util/queue.h"

void init_pwm(uint pin, uint clock_divider, uint high_limit, uint compare_value);
void init_button(uint8_t pin_nro);
void init_LED(uint pin_nro);
void update_LED_brightness(uint *led_pins, int size, uint16_t compare_value);
void rot_A_callback(uint gpio, uint32_t event);
bool monitor_toggle_button(bool led_on);

#define ROT_SW 12
#define ROT_A 10
#define ROT_B 11

#define MAX_BRIGHTNESS 1000
#define MIN_BRIGHTNESS 0
#define CLOCK_DIVIDER 125
#define HIGH_LIMIT 999
#define QUEUESIZE 10
#define BRIGHTEN 1
#define DIM 0
#define PWM_STEP 10

static queue_t events;

int main() {
    //Initialize buttons, leds and PWM
    uint buttons[] = {ROT_SW, ROT_A, ROT_B};
    uint led_pins[] = {20, 21, 22};
    int size = sizeof(led_pins) / sizeof(led_pins[0]);

    for (int i = 0; i < size; i++) {
        init_button(buttons[i]);
        init_LED(led_pins[i]);
        init_pwm(led_pins[i], CLOCK_DIVIDER, HIGH_LIMIT, 0);
    }
    gpio_pull_up(ROT_SW);

    // Initialize chosen serial port
    stdio_init_all();

    gpio_set_irq_enabled_with_callback(ROT_A, GPIO_IRQ_EDGE_RISE, true, rot_A_callback);

    uint16_t brightness = MAX_BRIGHTNESS;
    bool led_on = false;
    queue_init(&events, sizeof(int), QUEUESIZE);
    int turn = 0;

    // Loop forever
    while (true) {
        led_on = monitor_toggle_button(led_on); //determine led state (on/off)

        if (led_on){
            while (queue_try_remove(&events, &turn)) { //get values from interruption queue and determine if brighten or dim
                if (turn == BRIGHTEN && brightness < MAX_BRIGHTNESS) brightness += PWM_STEP;
                if (turn == DIM && brightness > MIN_BRIGHTNESS) brightness -= PWM_STEP;
            }
            update_LED_brightness(led_pins, size, brightness);
            printf("Brightness after update: %d\n", brightness);
        }
        else if (!led_on){
            update_LED_brightness(led_pins, size, MIN_BRIGHTNESS);
            if (brightness <= MIN_BRIGHTNESS){ //if leds are dimmed to 0%, set brightness level to 50%
                brightness = MAX_BRIGHTNESS / 2;
                led_on = true;
            }
            while (!queue_is_empty(&events)) {  //if not empty, empty interruption queue when led state is off
                queue_try_remove(&events, &turn);
                printf("%d", queue_get_level(&events));
            }
        }
        sleep_ms(2);
    }
}


void init_button(uint8_t pin_nro){
    gpio_init(pin_nro);
    gpio_set_dir(pin_nro, GPIO_IN);
}

void init_LED(uint pin_nro){
    gpio_init(pin_nro);
    gpio_set_dir(pin_nro, GPIO_OUT);
}

void init_pwm(uint pin, uint clock_divider, uint high_limit, uint compare_value){
    uint slice = pwm_gpio_to_slice_num(pin); //get slice
    uint_fast16_t channel = pwm_gpio_to_channel(pin); // get channel
    pwm_set_enabled(slice, false); //stop PWM if on
    pwm_config config = pwm_get_default_config(); //get default config
    pwm_config_set_clkdiv_int(&config, clock_divider); //set clock divider
    pwm_config_set_wrap(&config, high_limit); //set high limit
    pwm_init(slice, &config, false); //initialize pwm, dont start when initialization complete
    pwm_set_chan_level(slice, channel, compare_value); //set compare value to 0 in the beginning
    gpio_set_function(pin, GPIO_FUNC_PWM); //set pwm to pin
    pwm_set_enabled(slice, true); //start PWM
}

bool monitor_toggle_button(bool led_on){
    static bool pressed =false;
    if (gpio_get(ROT_SW) ==0 && !pressed) {
        pressed = true;
        led_on = !led_on; //toggle when pressing button
    }
    else if (gpio_get(ROT_SW) && pressed){ //debounce
        pressed = false;
    }
    return led_on;
}

void update_LED_brightness(uint *led_pins, int size, uint16_t compare_value){
    for (int i=0; i < size; i++) {
        uint slice = pwm_gpio_to_slice_num(led_pins[i]); //get slice
        uint channel = pwm_gpio_to_channel(led_pins[i]); // get channel
        pwm_set_chan_level(slice, channel, compare_value);
    }
}

void rot_A_callback(uint gpio, uint32_t event){
    int brighten_leds;
    if (gpio_get(ROT_B)){
        brighten_leds = DIM;
    }
    else{
        brighten_leds = BRIGHTEN;
    }
    queue_try_add(&events, &brighten_leds);
}

