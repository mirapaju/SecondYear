#ifndef LAB6_HOUSE_H
#define LAB6_HOUSE_H

using namespace std;

class House {
public:
    explicit House(const string &addr = "", double a = 0.0, int p = 0);
    friend ostream &operator<<(ostream &outs, const House &house); //output operator
    friend istream &operator>>(istream &ins, House &house); //input operator
    bool operator<(const House &other) const; //less than operator

    void setAddress(string &addr);
    void setArea(double a);
    void setPrice(int p);
    string getAddress() const;

private:
    string address;
    double area;
    int price;

};



#endif //LAB6_HOUSE_H
