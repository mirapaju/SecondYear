#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "ring_buffer.h"
#include "uart.h"
#include "stdio.h"

typedef struct {
    ring_buffer tx; //for transmit
    ring_buffer rx; //for receive
    uart_inst_t *uart; //pointer to uart_inst_t structure (UART instance, defined in Pico SDK)
    int irqn; //irq number for the UART
    irq_handler_t handler; //irq handler function for the UART
} uart_t;

void uart_irq_rx(uart_t *u);
void uart_irq_tx(uart_t *u);
void uart0_handler(void);
void uart1_handler(void);
static uart_t *uart_get_handle(int uart_nr);

static uart_t u0 = { .uart = uart0,
                     .irqn = UART0_IRQ,
                     .handler = uart0_handler };
static uart_t u1 = { .uart = uart1,
                     .irqn = UART1_IRQ,
                     .handler = uart1_handler };


static uart_t *uart_get_handle(int uart_nr) {
    return uart_nr ? &u1 : &u0; 
}

void uart_setup(int uart_nr, int tx_pin, int rx_pin, int speed)
{
    uart_t *uart = uart_get_handle(uart_nr);

    // ensure that we don't get any interrupts from the uart during configuration
    irq_set_enabled(uart->irqn, false);

    // allocate space for ring buffers
    rb_alloc(&uart->rx, 256); 
    rb_alloc(&uart->tx, 256);

    // Set up our UART with the required speed.
    uart_init(uart->uart, speed); 

    // Set the TX and RX pins by using the function select on the GPIO
    // See datasheet for more information on function select
    // Configuring a function for pins, tells the system that pin is used for UART transmission
    gpio_set_function(tx_pin, GPIO_FUNC_UART);
    gpio_set_function(rx_pin, GPIO_FUNC_UART);

    irq_set_exclusive_handler(uart->irqn, uart->handler);

    // Now enable the UART to send interrupts - RX only, function spesifically configures interrupt enables for UART
    uart_set_irq_enables(uart->uart, true, false);
    // enable UART0 interrupts on NVIC (nested vectored interrupt controller) enables interrupt in general
    irq_set_enabled(uart->irqn, true);
}

int uart_read(int uart_nr, uint8_t *buffer, int size) 
{
    int count = 0;
    uart_t *u = uart_get_handle(uart_nr); 
    printf("receive buffer content: ");
    rb_print(&u->rx);
    while(count < size && !rb_empty(&u->rx)) { 
        *buffer++ = rb_get(&u->rx);
        ++count;                       
    }

    return count; //return the amount of bytes read
}

int uart_write(int uart_nr, const uint8_t *buffer, int size)
{
    int count = 0;
    uart_t *u = uart_get_handle(uart_nr);
    while(count < size && !rb_full(&u->tx)){ //as long as transmission buffer is not full
        rb_put(&u->tx, *buffer++); //puts a byte from the source buffer into the transmit buffer and increments the source buffer pointer
        ++count;
    }
    printf("transmission buffer content: ");
    rb_print(&u->tx);
    // disable interrupts on NVIC while managing transmit interrupts
    irq_set_enabled(u->irqn, false);

    // if transmit interrupt is not enabled we need to enable it and give fifo an initial filling
    if(!(uart_get_hw(u->uart)->imsc & (1 << UART_UARTIMSC_TXIM_LSB))) {
        // enable transmit interrupt
        uart_set_irq_enables(u->uart, true, true);
        // fifo requires initial filling, no need for waiting data if interruption occurs
        uart_irq_tx(u);
    }

    // enable interrupts on NVIC
    irq_set_enabled(u->irqn, true);

    /* The transmit interrupt is typically generated when the UART is ready to accept more data for transmission.
     * Disabling the transmit interrupt when the transmit buffer is empty helps avoid unnecessary interrupt triggers
     * when there is no more data to send.*/

    return count;
}

int uart_send(int uart_nr, const char *str) 
{
    return uart_write(uart_nr, (const uint8_t *)str, strlen(str));
}


void uart_irq_rx(uart_t *u)
{
    while(uart_is_readable(u->uart)) { 
        uint8_t c = uart_getc(u->uart); 
        rb_put(&u->rx, c); 

    }
}

void uart_irq_tx(uart_t *u)
{
    while(!rb_empty(&u->tx) && uart_is_writable(u->uart)) { 
        uart_get_hw(u->uart)->dr = rb_get((&u->tx)); 
    }                                                 
    if (rb_empty(&u->tx)) {
        // disable tx interrupt if transmit buffer is empty, prevent unnecessary interrupts when there is no data
        uart_set_irq_enables(u->uart, true, false);
    }
}

void uart0_handler(void)
{
    uart_irq_rx(&u0);
    uart_irq_tx(&u0);
}

void uart1_handler(void)
{
    uart_irq_rx(&u1);
    uart_irq_tx(&u1);
}

bool wait_and_validate_response(int uart_nr, uint32_t timeout, int connect_attempts, const char *command, char *str,
                                int size, char *expected_str) {
    uart_t *u = uart_get_handle(uart_nr);
    char test_str[size];
    bool response_received = false;
    int attempts= 1;

   do{
        printf("Connection attempt nr: %d, attempt with: %s\n", attempts, command);
        uart_send(uart_nr, command);
        response_received = uart_is_readable_within_us(u->uart, timeout);
        if (response_received) {
            while (rb_empty(&u->rx)) sleep_ms(50);
            if (validate_response(uart_nr, test_str, size, expected_str)) {
                strcpy(str, test_str);
            }
            else response_received = false;
        }
        ++attempts;
    } while (attempts <= connect_attempts && !response_received);
    return response_received;
}

void flush_uart_buffer(int uart_nr) {
    uart_t *u = uart_get_handle(uart_nr);
    while (!rb_empty(&u->rx)) {
        rb_flush(&u->rx);
    }
}
