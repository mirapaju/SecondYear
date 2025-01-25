#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "uart.h"


#define STRLEN 80
#define BAUD_RATE 9600
#define TIMEOUT 500000

#if 0
#define UART_NR 0
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#else
#define UART_NR 1
#define UART_TX_PIN 4
#define UART_RX_PIN 5
#endif

bool monitor_toggle_button(uint pin);

enum State {
    STATE_WAIT_BUTTON_PRESS,
    STATE_SEND_AT_QUERY,
    STATE_SEND_VERSION_QUERY,
    STATE_SEND_DEVEUI_QUERY,
};

int main() {
    const uint button = 9;
    gpio_init(button);
    gpio_set_dir(button, GPIO_IN);
    gpio_pull_up(button);

    // Initialize chosen serial port
    stdio_init_all();

    uart_setup(UART_NR, UART_TX_PIN, UART_RX_PIN, BAUD_RATE);
    sleep_ms(1000);

    const char connection[] = "AT\r\n";
    const char version[] = "AT+VER\r\n";
    const char DEVEUI[] = "AT+ID=DEVEUI\r\n";
    char expected_at[STRLEN] = "+AT: OK\n";
    char expected_ver[STRLEN] = "+VER: 4.0.11\n";
    char expected_dev[STRLEN] = "+ID: DevEui, 2C:F7:F1:20:32:30:E7:94\n";
    char modified_devEui[20];
    char str[STRLEN];
    enum State state = STATE_WAIT_BUTTON_PRESS;

    while (true) {
        switch (state) {
            case STATE_WAIT_BUTTON_PRESS:
                if (monitor_toggle_button(button)) state = STATE_SEND_AT_QUERY;
                break;
            case STATE_SEND_AT_QUERY:
                if (wait_and_validate_response(UART_NR, TIMEOUT, 5, connection, str, STRLEN, expected_at)) {
                    printf("\n---------->Connected to LoRa Module\n");
                    state = STATE_SEND_VERSION_QUERY;
                }
                else {
                    printf("\n---------->Module not responding\n");
                    state = STATE_WAIT_BUTTON_PRESS;
                }
                break;
            case STATE_SEND_VERSION_QUERY:
                if (wait_and_validate_response(UART_NR, TIMEOUT, 1, version, str, STRLEN, expected_ver)) {
                    printf("\n--------->%s\n", str);
                    state = STATE_SEND_DEVEUI_QUERY;
                }
                else {
                    printf("\n---------->Module stopped responding\n");
                    state = STATE_WAIT_BUTTON_PRESS;
                }
                break;
            case STATE_SEND_DEVEUI_QUERY:
                if (wait_and_validate_response(UART_NR, TIMEOUT, 1, DEVEUI, str, STRLEN, expected_dev)) {
                    modify_devEui_response(str, modified_devEui);
                    printf("\n---------->%s\n", modified_devEui);
                }
                else {
                    printf("\n---------->Module stopped responding\n");
                }
                state = STATE_WAIT_BUTTON_PRESS;
                break;
        }
    }
}


bool monitor_toggle_button(uint pin) {
    static uint32_t last_press_time = 0;
    if (!gpio_get(pin)) {
        uint32_t current_time = time_us_32() / 1000;
        if (current_time - last_press_time > 300) {
            last_press_time = current_time;
            return true;  
        }
    }
    return false;
}