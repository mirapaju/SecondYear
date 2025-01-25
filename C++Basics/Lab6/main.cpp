#include <iostream>
#include "house.h"
#include <memory>
#include <vector>
#include <algorithm>

class Apartment {
public:
    explicit Apartment(shared_ptr<House> &housePtr, int aptNumber=0, double size=0, double rent=0)
            : houseptr(housePtr), apartment_num(aptNumber), size(size), rent(rent) {}
    void print() const;
    bool operator<(const Apartment& other) const;
private:
    shared_ptr<House> houseptr;
    int apartment_num;
    double size;
    double rent;
};

void Apartment::print() const{
    cout << "Address: " << houseptr->getAddress() << endl << "Apartment:" << apartment_num << endl
    << "Size: " << size << endl << "Rent: "  << rent << endl;
    cout << endl;
}

bool Apartment::operator<(const Apartment& other) const {
    return rent < other.rent;
}

int main() {
    auto house1 = make_shared<House>("Joku Tie 256", 400, 2800000);
    auto house2 = make_shared<House>("Joku Toinen Tie 47", 100, 250000);

    vector<Apartment> aprts;
    aprts.emplace_back(house1, 21, 58, 900);
    aprts.emplace_back(house1, 45, 85, 1100);
    aprts.emplace_back(house1, 2, 42, 750);
    aprts.emplace_back(house2, 1, 120, 1400);
    aprts.emplace_back(house2, 4, 90, 1200);
    aprts.emplace_back(house2, 6, 55, 950);
    aprts.emplace_back(house2, 3, 75, 1000);

    sort(aprts.begin(), aprts.end());
    for (const auto &apt : aprts) apt.print();
    return 0;
}
