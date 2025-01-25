#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pill_dispenser.h"
#include "pico/util/queue.h"

#define MAX_TURNS 7
#define PILL_FALL_TIME 84

static queue_t events;

void piezo_sensor_callback(uint gpio, uint32_t event){
    bool pill_dispensed =true;
    queue_try_add(&events, &pill_dispensed);
}

void dispense_state(Dispenser *dispenser){
    int steps_taken=0;
    bool event_handled =false;
    switch(dispenser->dispense_state){
        case IDLE: //idle is flag that no steps taken
            dispenser->dispense_state = FIRST_STEP;
        case FIRST_STEP: //first step is a flag that the movement has started, it is saved to eeprom
            move_motor_single_step(dispenser, CLOCKWISE);
            update_state_to_eeprom(dispenser, DEVICE_STATE_ADDR, DISPENSER_STATE_FORMAT);
            dispenser->dispense_state = REMAINING_STEPS;
        case REMAINING_STEPS: //here the actual dispensing happens
            gpio_set_irq_enabled(PIEZO_PIN, GPIO_IRQ_EDGE_FALL, true); //enable irq before moving the wheel
            while (steps_taken < dispenser->steps_in_turn-1) {
                move_motor_single_step(dispenser, CLOCKWISE);
                ++steps_taken;
                if (!event_handled && queue_try_remove(&events, &dispenser->pill_dispensed)){ //check if pill is dispensed while moving
                    update_state_to_eeprom(dispenser, DEVICE_STATE_ADDR, DISPENSER_STATE_FORMAT); //save state to eeprom
                    gpio_set_irq_enabled(PIEZO_PIN, GPIO_IRQ_EDGE_FALL, false); //disable the irq
                    event_handled =true;
                }
            }
            dispenser->dispense_state = COMPLETED_MOVEMENT;
            update_state_to_eeprom(dispenser, DEVICE_STATE_ADDR, DISPENSER_STATE_FORMAT); //save state to eeprom immediately when moving completed
        case COMPLETED_MOVEMENT: //handle irq
            sleep_ms(PILL_FALL_TIME); //wait for the pill to drop if not dropped while turning
            gpio_set_irq_enabled(PIEZO_PIN, GPIO_IRQ_EDGE_FALL, false); //disable irq
            while(queue_try_remove(&events, &dispenser->pill_dispensed)); //tyhjennä queue
        case EXIT: //dispensing data is valid here
            update_state_to_eeprom(dispenser, DEVICE_STATE_ADDR, DISPENSER_STATE_FORMAT);
            break;
    }
}


int main(){
    setup();
    stdio_init_all();

    gpio_init(22);
    gpio_set_dir(22, GPIO_OUT);

    queue_init(&events, sizeof(int), 10);
    Dispenser dispenser;
    init_dispenser(&dispenser);
    gpio_set_irq_enabled_with_callback(PIEZO_PIN, GPIO_IRQ_EDGE_FALL, true, piezo_sensor_callback);
    gpio_set_irq_enabled(PIEZO_PIN, GPIO_IRQ_EDGE_FALL, false);
    uint8_t data[LOG_ENTRY_SIZE];
    server_activation();
    printf("Start\n");

    while (true) {
        switch (dispenser.state) {
            case BOOT_RECOVER:
                set_states(&dispenser, STATE_REPORT_TO_SERVER, WAIT_BUTTON_PRESS); //default states if data or states not valid
                set_server_message(&server_msg, server_msg.wait_for_calibration);
                if (!read_and_validate_eeprom_data(DEVICE_STATE_ADDR, data, LOG_ENTRY_SIZE)) break;
                if (!valid_state_recovered(&dispenser, data)) break;
                server_msg_and_states_after_boot(&dispenser); //if data valid and states extracted to struct, go to check the states
                break;
            case STATE_REPORT_TO_SERVER: //send the changed state to server, also the data is sent to eeprom here bc after every state change this is the go to
                update_state_to_eeprom(&dispenser, DEVICE_STATE_ADDR, DISPENSER_STATE_FORMAT);
                printf("%s\n", server_msg.current_message);
                uart_puts(uart1, server_msg.current_message);
                dispenser.state = dispenser.next_state;
                update_state_to_eeprom(&dispenser, DEVICE_STATE_ADDR, DISPENSER_STATE_FORMAT);
                break;
            case WAIT_BUTTON_PRESS:
                printf("Press button.\n");
                while(!is_button_pressed()){
                    control_led(true);
                    sleep_ms(200);
                    control_led(false);
                    sleep_ms(200);
                }
                if (is_button_pressed()) {
                    printf("Button pressed for the first time.\n");
                    set_states(&dispenser, STATE_REPORT_TO_SERVER, CALIBRATE);
                    set_server_message(&server_msg, server_msg.calibrating);
                }
                break;
            case WAIT_SECOND_BUTTON_PRESS:
                gpio_put(LED_PIN, true);
                printf("Waiting fot second button press.\n");
                while (!is_button_pressed()) sleep_ms(200); //wait for the button to be pushed second time
                control_led(false);
                printf("Button pressed second time.\n");
                update_time_stamp(&dispenser);
                dispenser.timestamp = 0; //after button push the countdown is started
                update_time_stamp(&dispenser);
                set_states(&dispenser, STATE_REPORT_TO_SERVER, WAIT_FOR_DISPENSE);
                set_server_message(&server_msg, server_msg.wait_for_dispense);
                break;
            case CALIBRATE:
                calibrate(&dispenser);
                set_states(&dispenser, STATE_REPORT_TO_SERVER, WAIT_SECOND_BUTTON_PRESS);
                set_server_message(&server_msg, server_msg.calibrated);
                break;
            case STATE_RECALIBRATE:
               /* if(dispenser.pill_dispensed){ //if the pill was dispensed before power loss, recalibrate to the end of the compartment
                    recalibrate(&dispenser, dispenser.steps_in_turn*(dispenser.turns+1));
                    set_states(&dispenser, STATE_REPORT_TO_SERVER, AFTER_DISPENSE);
                    set_server_message(&server_msg, server_msg.recalibrated);
                    break;
                }*/
                 //if the pill (most likely) was not dispensed before power loss, recalibrate to the start of the compartment
                 //nyt tekee vaan tän riippumatta siitä, oliko pill dispensed ennen resettiä vai ei. ei oo vaarallista, mut server messaget on vähä hassut
                 //tosin nehän ei toimi muutenkaan :)
                recalibrate(&dispenser, dispenser.steps_in_turn * (dispenser.turns));
                set_server_message(&server_msg, server_msg.recalibrated);
                set_states(&dispenser, STATE_REPORT_TO_SERVER, DISPENSING); //try to dispense again after recalibration
                break;
            case WAIT_FOR_DISPENSE: //wait for the 30 seconds interval to pass
                if (dispenser.turns<MAX_TURNS) {
                    wait_dispensing_interval(&dispenser);
                    update_time_stamp(&dispenser);
                    dispenser.timestamp =0; //null the timestamp after 30 seconds
                    update_time_stamp(&dispenser);
                    set_server_message(&server_msg, server_msg.starting_to_dispense);
                    set_states(&dispenser, STATE_REPORT_TO_SERVER, DISPENSING); //report to server that the dispensing starts and then dispense
                    break;
                }
                else set_states(&dispenser,DISPENSER_EMPTY, STATE_REPORT_TO_SERVER);
                break;
            case DISPENSING: //start to dispense the pills
                dispense_state(&dispenser);
                dispenser.state = AFTER_DISPENSE;
                break;
            case AFTER_DISPENSE: //determine the next steps to take based on if the pill was dispensed or not
                ++dispenser.turns;
                dispenser.pill_dispensed ? set_server_message(&server_msg, server_msg.pill_dispensed) :
                set_server_message(&server_msg, server_msg.pill_not_dispensed);
                dispenser.pill_dispensed ? set_states(&dispenser, STATE_REPORT_TO_SERVER, STATE_WRITE_LOG) :
                set_states(&dispenser, BLINK_LED, STATE_REPORT_TO_SERVER); //if the pill is not dispensed, blink leds before going to reporting
                break;
            case BLINK_LED:
                for (int i =dispenser.led_blink_count; i < 5; i++){
                    control_led(true);
                    sleep_ms(200);
                    control_led(false);
                    sleep_ms(200);
                    save_led_blink_count(i);
                }
                save_led_blink_count(0);
                dispenser.led_blink_count = 0;
                set_states(&dispenser, STATE_REPORT_TO_SERVER, STATE_WRITE_LOG);
                break;
            case STATE_WRITE_LOG: //write the daily log to eeprom
                write_log_to_eeprom(&dispenser);
                dispenser.pill_dispensed = false; //null the dispensing flag
                dispenser.dispense_state = IDLE; //set dispensing state back to idle
                set_states(&dispenser, WAIT_FOR_DISPENSE, WAIT_FOR_DISPENSE);
                break;
            case DISPENSER_EMPTY:
                reset(&dispenser);
                set_states(&dispenser, STATE_REPORT_TO_SERVER, WAIT_BUTTON_PRESS);
                set_server_message(&server_msg, server_msg.dispenser_empty);
                break;
        }
        sleep_ms(50);
    }
    return 0;
}
