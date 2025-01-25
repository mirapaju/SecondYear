//
// Created by keijo on 4.11.2023.
//

#ifndef UART_IRQ_UART_H
#define UART_IRQ_UART_H


bool wait_and_validate_response(int uart_nr, uint32_t timeout, int connect_attempts, const char *command, char *str,
                                int size, char *expected_str);
void uart_setup(int uart_nr, int tx_pin, int rx_pin, int speed);
int uart_read(int uart_nr, uint8_t *buffer, int size);
int uart_write(int uart_nr, const uint8_t *buffer, int size);
int uart_send(int uart_nr, const char *str);

bool validate_response(int uart_nr, char *str, int size, char *expected_str);
void modify_devEui_response(const char *devEui, char *modified_DevEui);
void get_uart_strings(uart_inst_t uart_nr, char *str, int size);

#endif //UART_IRQ_UART_H
