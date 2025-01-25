#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <ctime>
#include <map>

//calls new but never calls delete ->leaks memory
//one dealer can advertise the car in multiple websites
//whenever dealer gets car->put car to vector and add it to all websites you have
//car is the key to the map and it associated with the dealer who is selling the car
//replace with smart pointers
//in each class there is destructor, but nothing ever gets deleted now bc nothing gets out of scope ->fix
//dealer sells a car-> removes a car from its own list ->website can detect when car is removed -> goes out of scope and gets destructed
//before printing check which of the pointers are removed/expired and get rid of them. then print

class Car {
	friend std::ostream &operator<<(std::ostream &out, const Car &car);
public:
	explicit Car(std::string m="Ford", std::string l="", int ml=0) : // Initializes model, possibly moving m
        model(std::move(m)),
        license(!l.empty()?std::move(l):(rand()%2?"FOO-":"BAR-") + std::to_string(rand()%999+1)),
        mileage(ml>0?ml:rand()%10000) {} // Initializes mileage, generating if not positive
	~Car() { std::cout << model << " " << license << " deleted" << std::endl; }; // Destructor prints deletion message
	void Read(); // Member function to read car information
	const std::string &GetLicense() const { return license; } // Getter for license
private:
	std::string model;
	std::string license;
	int mileage;
};

void Car::Read()
{
	// std::cout << "Enter car information" << std::endl;
	// replace the following with your own code
    //here you can enter car information, now there is just a fixed values bc Keijo is lazy, no need to change if you don't want to
	model = "Seat";
	license = (rand() % 2 ? "ZAP-" : "ZIP-") + std::to_string(rand() % 999 + 1);
	mileage = 10000 + rand() % 10000;
}

std::ostream & operator<<(std::ostream & out, const Car &car)
{
	std::cout << "Model: " << car.model << std::endl << "License: " << car.license << std::endl << "Mileage: " << car.mileage << std::endl;
	return out;
}

/* ---------------------- */

class Website {
public:
	 explicit Website(std::string n="carweb.com") : name(std::move(n)) {}
	~Website() { std::cout << name << " deleted" << std::endl; }; // Destructor prints deletion message
    void add(std::shared_ptr<Car> car, std::string &dealer) {
      //  std::weak_ptr<Car> wcar = car;
        listing[car] =dealer;
    }
    void print(std::ostream &out = std::cout);

private:
    std::map<std::weak_ptr<Car>, std::string, std::owner_less<std::weak_ptr<Car>>> listing;
    std::string name;
};


void Website::print(std::ostream &out) {
    out << name << std::endl;
    for (auto it = listing.begin(); it != listing.end();) {
        auto sharedCar = it->first.lock();
        if (sharedCar) { //weak_ptr is still valid, print the shared_ptr
            out << it->second << ":" << std::endl << *sharedCar;
            ++it;
        }
        else it = listing.erase(it);
    }
    out << name << " end of list" << std::endl;
}

class Dealer {
	friend std::ostream &operator<<(std::ostream &out, const Dealer &dealer);
public:
	explicit Dealer(std::string name_ = "John Doe") : name(std::move(name_)) {}; //initializes dealer name
	~Dealer() { std::cout << name << " deleted" << std::endl; }; // prints deletion message
	void buy(); //simulate buying a car
	void sell(); //simulate selling a car
    //NEW
    void add(std::shared_ptr<Car> car){
        cars.push_back(car);
        for (auto &site : sites) site->add(car, name);
    }
    //NEW
    void add_site(std::shared_ptr<Website> w) {sites.push_back(w);};
private:
	std::string name;//dealer name
    //NEW
    std::vector<std::shared_ptr<Car>> cars;
    std::vector<std::shared_ptr<Website>> sites;
    //
};

void Dealer::buy()
{//simulates buying a car by creating a new Car object and adding it to the dealer's list
    //NEW
    auto car = std::make_shared<Car>();
	car->Read();
	add(car);
}

void Dealer::sell()
{// simulates selling a car by asking for its license and removing it from the dealer's list and associated websites
	std::cout << *this; // print my list of cars
	std::cout << "Enter license of car you want to buy: " << std::flush;

	std::string license;
	std::cin >> license;
    //NEW
    auto ci = std::find_if(cars.begin(), cars.end(), [&license](const auto &c) {return license == c->GetLicense(); });
    if (ci != cars.end()) {
        cars.erase(ci);
    }
}

std::ostream & operator<<(std::ostream & out, const Dealer & dealer)
{//overloads the << operator to print Dealer's car list
	std::cout << dealer.name << "'s cars for sale" << std::endl;
	for (const auto &car : dealer.cars) std::cout << *car;
	std::cout << "End of " << dealer.name << "'s cars listing" << std::endl;

	return out;
}

void car_sales() {
    std::cout << "Car sales started" << std::endl;

    auto wa = std::make_shared<Website>("www.autos.com");
    auto wb = std::make_shared<Website>("www.bilar.com");
    auto a = std::make_shared<Dealer>("Alan Aldis");
    auto b = std::make_shared<Dealer>("Bill Munny");

    {
        auto wc = std::make_shared<Website>("www.cars.com");
        auto c = std::make_shared<Dealer>("Casey Ball");

        auto ca = std::make_shared<Car>();
        auto cb = std::make_shared<Car>();

        a->add_site(wa);
        a->add_site(wb);
        b->add_site(wb);
        b->add_site(wc);
        c->add_site(wa);
        c->add_site(wb);
        c->add_site(wc);

        a->buy();
        a->buy();
        a->buy();
        a->buy();

        b->buy();
        b->buy();
        b->buy();

        c->buy();
        c->buy();
        c->add(ca);
        c->add(cb);
        wa->print();
        wb->print();
        wc->print();

        std::cout << *a << *b << *c << std::endl;

        a->sell();

        std::cout << *a << *b << *c << std::endl;

        wa->print();
        wb->print();
        wc->print();

    }

    wa->print();
    wb->print();

    std::cout << "Car sales ended" << std::endl;
}

int main(int argc, char **argv) {

	srand(time(nullptr));

	car_sales();

	return 0;
}


