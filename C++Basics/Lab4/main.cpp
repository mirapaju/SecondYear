#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

class House {
public:
    explicit House(const string &addr = "", double a = 0.0, int p = 0) : address(addr), area(a), price(p) {}
    friend ostream &operator<<(ostream &outs, const House &house); //output operator
    friend istream &operator>>(istream &ins, House &house); //input operator
    bool operator<(const House &other) const; //less than operator

    void setAddress(string &addr) { address = addr; }

    void setArea(double a) { area = a; }

    void setPrice(int p) { price = p; }

private:
    string address;
    double area;
    int price;

};

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

bool House::operator<(const House &other) const{
    return (price / area) < (other.price / other.area);
}

int main() {
    vector<House> houses;

    House house1;
    string addr;
    double area;
    int price;
    string input;
    cout << "Enter information for house nr 1 (address, area, price):";
    getline(cin, input);
    istringstream ss(input);

    getline(ss, addr, ',');
    ss >> area;
    ss.ignore();
    ss >> price;



    house1.setAddress(addr);
    house1.setArea(area);
    house1.setPrice(price);
    houses.emplace_back(house1);

    string in2;
    cout << "Enter information for house nr 2 (address, area, price):";
    getline(cin, in2);
    istringstream ss2(in2);

    getline(ss2, addr, ',');
    ss2 >> area;
    ss2.ignore();
    ss2 >> price;

    House house2(addr, area, price);
    houses.emplace_back(house2);

    for (int i=3; i<6; i++){
        cout << "Enter information for house nr " << i << " (address, area, price):";
        House house;
        cin >> house;
        houses.emplace_back(house);
    }

    sort(houses.begin(), houses.end()); 
    for (const auto &house : houses) cout << house << endl; 

    return 0;
}


