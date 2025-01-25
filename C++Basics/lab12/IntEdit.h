//
// Created by Keijo Länsikunnas on 5.3.2024.
//

#ifndef LAB12_INTEDIT_H
#define LAB12_INTEDIT_H
#include <string>
#include <cstdint>
#include <vector>
#include <iostream>
#include "MenuItem.h"


class Observer {
public:
    virtual void Update(int value) = 0;
};

class Subject {
public:
    virtual void Attach(Observer *o) = 0; // Set
    virtual void Notify() = 0;
};

class ColorObserver : public Observer {
public:
    void Update(int value) override {
        std::cout << "Color value changed: " << value << std::endl;
    }
};

class IntEdit : public MenuItem, public Subject {
public:
    explicit IntEdit(std::string menu_title);
    bool event(menuEvent e) override;
    void show() override;
    void Attach(Observer* o) override {observers.push_back(o);}
    void Notify() override {for (Observer* observer : observers) {
        std::cout << "notifying" << std::endl;
        observer->Update(value);}}
protected:
    int value;
    int backup;
    bool changed;
    std::vector<Observer*> observers;
};


#endif //LAB12_INTEDIT_H
