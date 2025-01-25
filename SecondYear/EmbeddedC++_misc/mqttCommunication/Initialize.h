//
// Created by mirap on 4/23/2024.
//

#ifndef EXERCISE4_INITIALIZE_H
#define EXERCISE4_INITIALIZE_H

#include <string>
#include "pico/stdlib.h"
#include <map>
#include <memory>

#define LED_1 20
#define LED_2 21
#define LED_3 22
#define BUTTON_PIN 9
#define DISCONNECT_BUTTON 7

class Led{
public:
    explicit Led(int pin){
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);
    }
private:
};

class Button{
public:
    explicit Button(int pin){
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_IN);
        gpio_pull_up(pin);
    }
};

#endif //EXERCISE4_INITIALIZE_H
