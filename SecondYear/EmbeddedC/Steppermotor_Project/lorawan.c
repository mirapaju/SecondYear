#include "pill_dispenser.h"
#include "pico/stdlib.h"
#include "pico/time.h"


// initialization of the messages
Message server_msg = {
        .current_message = "DEFAULT MESSAGE",
        .boot = "at+MSG=\"BOOT\"\r\n",
        .data_recovered = "at+MSG=\"VALID STATE RECOVERED ON BOOT, CONTINUING WHERE LEFT.\"\r\n",
        .wait_for_calibration = "at+MSG=\"NO VALID DATA RECOVERED, WAITING FOR CALIBRATION\"\r\n",
        .power_off_while_calibration = "at+MSG=\"POWERED OFF WHILE CALIBRATION, STARTING TO CALIBRATE\"\r\n",
        .power_off_while_recalibration = "at+MSG=\"POWERED OFF WHILE RECALIBRATION, STARTING TO CALIBRATE\"\r\n",
        .powered_off_after_dispense = "at+MSG=\"POWERED OFF AFTER DISPENSING, NO DISPENSING DATA RECOVERED\"\r\n",
        .powered_off_while_reporting = "at+MSG=\"POWER LOSS WHILE REPORTING TO SERVER, CONTINUE FROM NEXT STEP\"\r\n",
        .calibrating = "at+MSG=\"STARTING TO CALIBRATE\"\r\n",
        .calibrated = "at+MSG=\"CALIBRATED. WAITING THE DISPENSER TO BE FILLED\"\r\n",
        .recalibrate_dispensed = "at+MSG=\"POWER OFF WHILE DISPENSING, RECALIBRATING. PILL DISPENSED\"\r\n",
        .recalibrate_not_dispensed = "at+MSG=\"POWER OFF WHILE DISPENSE, RECALIBRATING AND TRYING TO DISPENSE\"\r\n",
        .recalibrated = "at+MSG=\"RECALIBRATED\"\r\n",
        .wait_for_dispense = "at+MSG=\"DISPENSE INTERVAL COUNTDOWN STARTED\"\r\n",
        .starting_to_dispense = "at+MSG=\"STARTING TO DISPENSE PILLS\"\r\n",
        .pill_dispensed = "at+MSG=\"PILL DISPENSED\"\r\n",
        .pill_not_dispensed = "at+MSG=\"PILL NOT DISPENSED\"\r\n",
        .dispenser_empty = "at+MSG=\"DISPENSER EMPTY\"\r\n",
};

void set_server_message(struct Server_messages *message_instance, const char *message) {
    message_instance->current_message = message;
}

void server_activation() { //Tekee yhteyden lowaren netwörkkiin
    uint8_t response[LOG_MSG_SIZE];
    const char at_command[] = "at\r\n";
    const char mode[] = "at+MODE=LWOTA\r\n";
    const char appkey[] = "at+KEY=APPKEY, 82fe89bd6639d77a02a4f04ca6b51031\r\n";
    const char class[] = "at+CLASS=A\r\n";
    const char port[] = "at+PORT=8\r\n";
    const char join_command[] = "at+JOIN\r\n";

    uart_init(uart1, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    uart_puts(uart1, at_command);
    uart_puts(uart1, mode);
    uart_puts(uart1, appkey);
    uart_puts(uart1, class);
    uart_puts(uart1, port);
    uart_puts(uart1, join_command);
}