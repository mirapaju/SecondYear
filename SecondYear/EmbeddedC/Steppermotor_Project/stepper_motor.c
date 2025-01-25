#include "pill_dispenser.h"
#include "pico/stdlib.h"
#include "pico/time.h"

#define IN1 2
#define IN2 3
#define IN3 6
#define IN4 13
#define NUM_COILS 4
#define STEPPING_DELAY 2
#define CALIBRATION_RUNS 3

//initialize the dispenser
void init_dispenser(Dispenser *dispenser) {
    static const int sequence[][NUM_COILS] = {
            {1, 0, 0, 0}, //A
            {1, 1, 0, 0}, //AB
            {0, 1, 0, 0}, //B
            {0, 1, 1, 0}, //BC
            {0, 0, 1, 0}, //C
            {0, 0, 1, 1}, //CD
            {0, 0, 0, 1}, //D
            {1, 0, 0, 1}  //AD
    };
    dispenser->state = BOOT_RECOVER;
    dispenser->dispense_state=IDLE;
    dispenser->step_sequence = sequence;
    dispenser->steps_in_turn =0;
    dispenser->current_sequence = 0;
    dispenser->turns = 0;
    dispenser->dispense_entry_addr=DISPENSE_LOG_START_ADDR;
    dispenser->pill_dispensed =0;
    dispenser->timestamp =0;
    dispenser->led_blink_count=0;


    //initialize coil pins and copy the coil configuration into the struct
    int coil_pins[4] = {IN1,IN2,IN3,IN4};
    for (int i = 0; i < 4; i++) {
        gpio_init(coil_pins[i]);
        gpio_set_dir(coil_pins[i], GPIO_OUT);
        dispenser->coils[i] = coil_pins[i];
    }
};

//reset dispenser conf after all pills dispensed
void reset(Dispenser *dispenser){
    dispenser->state = WAIT_BUTTON_PRESS;
    dispenser->dispense_state = IDLE;
    dispenser->steps_in_turn =0;
    dispenser->current_sequence = 0;
    dispenser->turns = 0;
    dispenser->pill_dispensed =false;
    dispenser->timestamp =0;
}

//run stepper motor in given direction until sensor encounters given signal
int run_until_signal_change(Dispenser *dispenser, int direction, bool signal_high){
   int steps =0;
    while (gpio_get(OPTO_FORK_PIN) == signal_high){
        move_motor_single_step(dispenser, direction);
        ++steps;
    }
    return steps;
}

//move stepper motor one step
void move_motor_single_step(Dispenser *dispenser, int direction) {
    int sequence = dispenser->current_sequence; //start from the last saved sequence
    for (int i = 0; i < NUM_COILS; i++) gpio_put(dispenser->coils[i], dispenser->step_sequence[sequence][i]);
    sleep_ms(STEPPING_DELAY);
    sequence = (sequence + direction + 8) % 8;
    dispenser->current_sequence = sequence; //save the sequence
}


void calibrate(Dispenser *dispenser) {
    int steps_taken=0;
    run_until_signal_change(dispenser, CLOCKWISE, true); //find the gap
    for (int runs = 0; runs < CALIBRATION_RUNS; runs++) { //run motor three rounds, just bc we can
        steps_taken += run_until_signal_change(dispenser, CLOCKWISE, false);
        steps_taken += run_until_signal_change(dispenser, CLOCKWISE, true);
    }
    //save calibration settings to struct and align the dispenser compartment with the drop tube
    dispenser->steps_in_turn = (steps_taken / CALIBRATION_RUNS) / 8;
    int steps_for_alignment = dispenser->steps_in_turn / 3; 
    for (int i=0; i<steps_for_alignment; i++) move_motor_single_step(dispenser, CLOCKWISE);
}


void recalibrate(Dispenser *dispenser, int steps) {
    run_until_signal_change(dispenser, COUNTERCLOCKWISE, true);
    int alignment_steps = dispenser->steps_in_turn / 3;
    for (int i = 0; i < alignment_steps; ++i) move_motor_single_step(dispenser,COUNTERCLOCKWISE);
    for (int i = 0; i < steps; ++i) move_motor_single_step(dispenser,CLOCKWISE);
}


