#include <stdio.h>
#include "pill_dispenser.h"
#include "hardware/i2c.h"
#include "string.h"
#include "stdbool.h"

#define DEVADDR 0x50
#define CRC_SIZE 2
#define LOG_FULL 0


void erase_log() {
    uint8_t data_to_write[3] ={(uint8_t)(DISPENSE_LOG_START_ADDR >> 8), (uint8_t)DISPENSE_LOG_START_ADDR, 0};
    uint16_t pos=DISPENSE_LOG_START_ADDR;
    while(pos < DISPENSE_LOG_MAX_ADDR){
        data_to_write[0] = (uint8_t)(pos >> 8);
        data_to_write[1] = (uint8_t)pos;
        i2c_write_blocking(i2c1, DEVADDR, data_to_write, 3, false);
        sleep_ms(25);
        pos += LOG_ENTRY_SIZE;
    }
}

bool validate_log_entry(const uint8_t *data_from_EEPROM) {
    bool null_found=false;
    if (data_from_EEPROM[0] == '\0') return false; //first character of msg null, address is invalid

    for (int i=0; i<LOG_MSG_SIZE && !null_found; i++){
        if (data_from_EEPROM[i] == '\0') null_found =true;
    }
    if (!null_found) return false; //if no null in 62 characters, address is invalid

    int length = strlen((char*)data_from_EEPROM) + 1 + CRC_SIZE;
    if (crc16(data_from_EEPROM, length) != 0) return false;
    return  true;
}

//searches for the next empty memory address in the given address space, DOES NOT ERASE LOG IF ALL ADDRESSES ARE RESERVED
uint16_t find_empty_address(uint16_t start_addr, uint16_t max_addr) {
    uint16_t addr = start_addr;
    while (addr < max_addr){
        uint8_t data_from_EEPROM[LOG_ENTRY_SIZE];
        read_and_validate_eeprom_data(addr, data_from_EEPROM, LOG_ENTRY_SIZE);

        if (!validate_log_entry(data_from_EEPROM)) return addr; //return the empty address found
        addr += LOG_ENTRY_SIZE;
    }
    return LOG_FULL; //if all addresses are reserved, return notification
}

//searches for the next empty memory address in given address space, if not found erases the log and returns the first address
uint16_t find_empty_address_with_rollover(uint16_t start_addr, uint16_t max_addr) {
    uint16_t address = find_empty_address(start_addr, max_addr);
    if (address == LOG_FULL) {
        erase_log();
        return start_addr;
    }
    else return address;
}

void write_uint16_to_buffer(uint16_t value, uint8_t* buffer) {
    buffer[0] = (uint8_t)(value >> 8);  
    buffer[1] = (uint8_t)value;        
}

void write_entry(char *data, size_t data_size, uint16_t address) {
    size_t address_size = sizeof(address);
    uint8_t data_to_write[address_size + data_size + 1 + CRC_SIZE];

    write_uint16_to_buffer(address, data_to_write); //copy the eeprom data address to buffer
    memcpy(data_to_write + address_size, data, data_size); //copy the message to buffer, right after the data address
    data_to_write[address_size + data_size] = '\0'; //add terminating null just to be sure
    uint16_t crc = crc16(data_to_write + address_size, data_size+1);
    write_uint16_to_buffer(crc, data_to_write + address_size + data_size+1); //append CRC to the buffer


    i2c_write_blocking(i2c1, DEVADDR, data_to_write, address_size + data_size + 1 + CRC_SIZE, false);
    sleep_ms(5);
}


void update_state_to_eeprom(Dispenser *dispenser, uint16_t address, char *format){
    int size = LOG_MSG_SIZE *2;
    char str[size];
    update_time_stamp(dispenser);
    int snprintf_result = snprintf(str, (size), format, dispenser->state, dispenser->next_state, dispenser->dispense_state,
                                   dispenser->current_sequence, dispenser->turns, dispenser->steps_in_turn, dispenser->timestamp,
                                   (int)dispenser->pill_dispensed);

    if (snprintf_result < 0 || snprintf_result > LOG_MSG_SIZE) strcpy(str, "INVALID DATA UPDATE.");

    write_entry(str, strlen(str), address); //save to eeprom
}


bool read_and_validate_eeprom_data(uint16_t address, uint8_t *data, int size) {
    uint8_t buffer[2];
    write_uint16_to_buffer(address, buffer);
    i2c_write_blocking(i2c1, DEVADDR, buffer, sizeof(buffer), true);
    i2c_read_blocking(i2c1, DEVADDR, data, size, false);
    sleep_ms(5);
    if (!validate_log_entry(data)) return false;
    else return true;
}

//write the daily log to next empty address
void write_log_to_eeprom(Dispenser *dispenser){
    char pill_log_entry[LOG_MSG_SIZE];
    snprintf(pill_log_entry, LOG_MSG_SIZE, "Day:%d. %s Pills left to dispense: %d.", dispenser->turns,
             dispenser->pill_dispensed ? "Pill dispensed." : "Pill not dispensed.",
             7 - dispenser->turns);
    printf("Log message: %s\n", pill_log_entry);
    int addr = find_empty_address_with_rollover(dispenser->dispense_entry_addr, DISPENSE_LOG_MAX_ADDR); //find address for daily dispensing log and write in it
    write_entry(pill_log_entry, strlen(pill_log_entry), addr);
    dispenser->dispense_entry_addr = addr + LOG_ENTRY_SIZE; //update daily log address after writing
}


void save_led_blink_count(int count){
    uint8_t data_to_write[4] = {(uint8_t)(LED_BLINK_ADDRESS >> 8), (uint8_t)LED_BLINK_ADDRESS, count, ~count};
    i2c_write_blocking(i2c1, DEVADDR, data_to_write, 4, false);
    sleep_ms(5);
}

bool valid_led_blink_count_recovered(Dispenser *dispenser) {
    uint8_t mem_address[2] = {(uint8_t) (LED_BLINK_ADDRESS >> 8), (uint8_t) LED_BLINK_ADDRESS};
    uint8_t data[2] = {0};

    i2c_write_blocking(i2c1, DEVADDR, mem_address, 2, true);
    i2c_read_blocking(i2c1, DEVADDR, data, 2, false);
    if (data[0] == (uint8_t)~data[1]){
        dispenser->led_blink_count = data[0];
        return true;
    }
    return false;
}






