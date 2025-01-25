#ifndef SENSOR_PILL_DISPENSER_H
#define SENSOR_PILL_DISPENSER_H
#include "stdbool.h"
#include "stdint.h"
#include <stddef.h>

#define LOG_ENTRY_SIZE 64
#define SERVER_MSG_SIZE 128
#define LOG_MSG_SIZE (LOG_ENTRY_SIZE-2) //assuming msg contains terminating null
#define DISPENSE_LOG_START_ADDR 0
#define DISPENSE_LOG_MAX_ADDR 2048
#define DEVICE_STATE_ADDR (DISPENSE_LOG_MAX_ADDR + LOG_ENTRY_SIZE)
#define LED_BLINK_ADDRESS 2240

#define PIEZO_PIN 27
#define OPTO_FORK_PIN 28
#define LED_PIN 22
#define BUTTON_PIN 9
#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define BAUD_RATE 9600

#define CLOCKWISE 1
#define COUNTERCLOCKWISE (-1)
#define DISPENSE_INTERVAL (30 * 1000000)
#define DISPENSER_STATE_FORMAT "ST:%d;NXT:%d;DISP_ST:%d;SEQ:%d;TURN:%d;STPS:%d;TIME:%llu;D:%d;"

typedef enum State {
    STATE_REPORT_TO_SERVER,
    BOOT_RECOVER,
    CALIBRATE,
    WAIT_BUTTON_PRESS,
    WAIT_SECOND_BUTTON_PRESS,
    WAIT_FOR_DISPENSE,
    DISPENSING,
    AFTER_DISPENSE,
    BLINK_LED,
    STATE_WRITE_LOG,
    STATE_RECALIBRATE,
    DISPENSER_EMPTY,
} Device_state;

typedef enum Dispense_state {
    IDLE,
    FIRST_STEP, //tässä saa kiinni siitä että on liikuttu yks askel
    REMAINING_STEPS,
    COMPLETED_MOVEMENT,
    EXIT,
} Dispense_state;

typedef struct Pill_dispenser{
    Device_state state;
    Device_state next_state;
    Dispense_state dispense_state;
    const int (*step_sequence)[4];
    int coils[4];
    int steps_in_turn;
    int current_sequence;
    int turns;
    bool pill_dispensed;
    uint16_t dispense_entry_addr;
    uint64_t timestamp;
    int led_blink_count;
} Dispenser;


typedef struct Server_messages {
    const char *current_message;
    const char boot[SERVER_MSG_SIZE];
    const char data_recovered[SERVER_MSG_SIZE];
    const char power_off_while_calibration[SERVER_MSG_SIZE];
    const char power_off_while_recalibration[SERVER_MSG_SIZE];
    const char powered_off_after_dispense[SERVER_MSG_SIZE];
    const char powered_off_while_reporting[SERVER_MSG_SIZE];
    const char wait_for_calibration[SERVER_MSG_SIZE];
    const char calibrating[SERVER_MSG_SIZE];
    const char calibrated[SERVER_MSG_SIZE];
    const char recalibrate_dispensed[SERVER_MSG_SIZE];
    const char recalibrate_not_dispensed[SERVER_MSG_SIZE];
    const char recalibrated[SERVER_MSG_SIZE];
    const char wait_for_dispense[SERVER_MSG_SIZE];
    const char pill_dispensed[SERVER_MSG_SIZE];
    const char pill_not_dispensed[SERVER_MSG_SIZE];
    const char dispenser_empty[SERVER_MSG_SIZE];
    const char starting_to_dispense[SERVER_MSG_SIZE];
} Message;

extern Message server_msg;

//stepper_motor.c:
void init_dispenser(Dispenser *dispenser);
void calibrate(Dispenser *dispenser);
void recalibrate(Dispenser *dispenser, int steps);
void move_motor_single_step(Dispenser *dispenser, int direction);
void reset(Dispenser *dispenser);

//eeprom.c:
uint16_t find_empty_address(uint16_t start_addr, uint16_t max_addr);
bool validate_log_entry(const uint8_t *data_from_EEPROM);
bool read_and_validate_eeprom_data(uint16_t address, uint8_t data[], int size);
void erase_log();
void write_entry(char *data, size_t data_size, uint16_t address);
uint16_t find_empty_address_with_rollover(uint16_t start_addr, uint16_t max_addr);
void update_state_to_eeprom(Dispenser *dispenser, uint16_t address, char *format);
void write_log_to_eeprom(Dispenser *dispenser);
void save_led_blink_count(int count);
bool valid_led_blink_count_recovered(Dispenser *dispenser);


//states.c:
bool valid_state_recovered(Dispenser *dispenser, uint8_t *data);
void set_states(Dispenser *dispenser, Device_state state, Device_state next_state);
void server_msg_and_states_after_boot(Dispenser *dispenser);


//misc:
void setup();
uint64_t update_time_stamp(Dispenser *dispenser);
void wait_dispensing_interval(Dispenser *dispenser);
uint16_t bytes_to_uint16(uint8_t high_byte, uint8_t low_byte);
uint16_t crc16(const uint8_t *data, size_t length);
void control_led(bool on);
bool is_button_pressed();

//lorawan:
void set_server_message(struct Server_messages *message_instance, const char *message);
void server_activation();

#endif //SENSOR_PILL_DISPENSER_H
