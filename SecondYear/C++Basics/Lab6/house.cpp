#include <iostream>
#include <sstream>
#include "house.h"

House::House(const string &addr, double a, int p) : address(addr), area(a), price(p) {}

ostream& operator<<(ostream &outs, const House &house){
    outs << house.address << ", " << house.area << ", " << house.price;
    return outs;
}

istream& operator>>(istream &ins, House &house){
    string line;
    getline(ins, line);
    istringstream ss(line);
    getline(ss, house.address, ',');

    ss >> house.area;
    ss.ignore();
    ss >> house.price;
    return ins;
}

void House::setAddress(string &addr) { address = addr; }

void House::setArea(double a) { area = a; }

void House::setPrice(int p) { price = p; }

bool House::operator<(const House &other) const{
    return (price / area) < (other.price / other.area);
}

string House::getAddress() const{
    return address;
}
