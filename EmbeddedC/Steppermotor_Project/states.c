#include "pill_dispenser.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>


//extract the dispenser configurations from eeprom data to struct
bool valid_state_recovered(Dispenser *dispenser, uint8_t *data){
    char* token = strtok((char*)data, ";");

    while (token != NULL) {
        char key[20];
        int value;
        if (sscanf(token, "%[^:]:%d", key, &value) != 2) return false;

        if (strcmp(key, "ST") == 0) dispenser->state = value;
        else if (strcmp(key, "NXT") == 0) dispenser->next_state = value;
        else if (strcmp(key, "DISP_ST") == 0) dispenser->dispense_state = value;
        else if (strcmp(key, "SEQ") == 0) dispenser->current_sequence = value;
        else if (strcmp(key, "TURN") == 0) dispenser->turns = value;
        else if (strcmp(key, "STPS") == 0) dispenser->steps_in_turn = value;
        else if (strcmp(key, "TIME") == 0) dispenser->timestamp = strtoull(strchr(token, ':') + 1, NULL, 10);
        else if (strcmp(key, "D") == 0) dispenser->pill_dispensed = value;
        token = strtok(NULL, ";");
    }
    return true;
}

//set next and current state of the dispenser
void set_states(Dispenser *dispenser, Device_state state, Device_state next_state){
    dispenser->next_state = next_state;
    dispenser->state = state;
}


void powered_off_while_dispensing(Dispenser *dispenser) {
    switch (dispenser->dispense_state){ 
        case IDLE: //no steps taken, ok to continue where left off
            set_states(dispenser, STATE_REPORT_TO_SERVER, DISPENSING);
            set_server_message(&server_msg, server_msg.data_recovered);
            return;
        case COMPLETED_MOVEMENT: //power off after turn, dispensing data might not be valid but no need for recalibration
            if(!dispenser->pill_dispensed) {
                ++dispenser->turns; 
                dispenser->dispense_state = IDLE;
                int addr = find_empty_address_with_rollover(dispenser->dispense_entry_addr, DISPENSE_LOG_MAX_ADDR);
                write_entry((char *) server_msg.powered_off_after_dispense,
                            strlen(server_msg.powered_off_after_dispense), addr);
                dispenser->dispense_entry_addr = addr + LOG_ENTRY_SIZE; //update daily log address after writing
                set_states(dispenser, STATE_REPORT_TO_SERVER, WAIT_FOR_DISPENSE);
                set_server_message(&server_msg, server_msg.powered_off_after_dispense);
                return;
            }
            else{
                dispenser->dispense_state = EXIT;
                break;
            }
        case EXIT: //power off after handling the interruption, pill dispensed/not dispensed data is valid
            set_states(dispenser, STATE_REPORT_TO_SERVER, AFTER_DISPENSE);
            set_server_message(&server_msg, server_msg.data_recovered);
            return;
        default:
            if (dispenser->pill_dispensed){//pill dispensed while turning, not certain if turned to the end so recalibration needed
                set_states(dispenser, STATE_REPORT_TO_SERVER, STATE_RECALIBRATE);
                set_server_message(&server_msg, server_msg.recalibrate_dispensed);
                return;
            }
            else{ //motor started to turn but did not reach movement_complete or exit and the pill probably wasn't dispensed while moving, recalibration
                set_states(dispenser, STATE_REPORT_TO_SERVER, STATE_RECALIBRATE);
                set_server_message(&server_msg, server_msg.recalibrate_not_dispensed);
                return;
            }
    }
}

void server_msg_and_states_after_boot(Dispenser *dispenser){
        switch(dispenser->state){
            case CALIBRATE: //powered off while calibration
                set_states(dispenser, STATE_REPORT_TO_SERVER, CALIBRATE);
                set_server_message(&server_msg, server_msg.power_off_while_calibration);
                return;
            case STATE_RECALIBRATE: //powered off while recalibration
                set_states(dispenser, STATE_REPORT_TO_SERVER, STATE_RECALIBRATE);
                set_server_message(&server_msg, server_msg.power_off_while_recalibration);
                return;
            case STATE_REPORT_TO_SERVER: //powered off while reporting
                set_server_message(&server_msg, server_msg.powered_off_while_reporting);
                return;
            case WAIT_FOR_DISPENSE: //powered off while waiting, make sure the waiting interval has not passed
                dispenser->state = STATE_REPORT_TO_SERVER;
                dispenser->next_state = (update_time_stamp(dispenser) >= DISPENSE_INTERVAL) ?
                                        DISPENSING : WAIT_FOR_DISPENSE;
                set_server_message(&server_msg, server_msg.data_recovered);
                return;
            case DISPENSING: //powered off while in dispensing state, check if steps were taken and handle that
                powered_off_while_dispensing(dispenser);
                return;
            case BLINK_LED:
                set_states(dispenser, STATE_REPORT_TO_SERVER, AFTER_DISPENSE); //go back to previous state to get the messages for log and server
                --dispenser->turns; 
                set_server_message(&server_msg, server_msg.data_recovered);
                valid_led_blink_count_recovered(dispenser); //start to blink where left off before power off, if no blink count found just start from zero
            default: //the states that doesn't require additional checking of conditions can continue as left
                set_server_message(&server_msg, server_msg.data_recovered);
                dispenser->next_state = dispenser->state;
                dispenser->state = STATE_REPORT_TO_SERVER;
                return;
        }
}

