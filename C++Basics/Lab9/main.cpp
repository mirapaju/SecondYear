#include <iostream>
#include <string>
#include <vector>

using namespace std;

//improve ex7 by using observer pattern to report when a spy’s resistance is broken. Add the following abstract class definitions:

class Observer;
class Subject {
public:
    virtual void Attach(Observer *o) = 0; // Set
    virtual void Notify() = 0;
};
// Abstract interface of Observer
class Observer {
public:
    virtual void Update() = 0;
};
//unchanged:
class Person {
public:
	explicit Person(const char* name = "nobody") : name(name){};
	virtual ~Person() = default;
	virtual void identity() const {cout << "My name is: " << name << endl;};
	virtual void interrogate(){};
private:
	string name;
};


class Judge: public Person, public Observer{
public:
    explicit Judge(const char* name) : Person(name){};
    void Update() override {
        cout << "[ ";
        Person::identity();
        cout << "I'm sending a spy to prison!]" << endl;}
};


class Spy: public Person, public Subject {
public:
    Spy(const char *name, const char *fake_name, int resistance)
    : Person(name), alias(fake_name), resistance(resistance) {};
    void set_identity(const char *fake_name) {alias = fake_name;};
    void identity() const override;
    void interrogate() override;
    //new part for this lab, Spy is a subject:
    void Attach(Observer* o) override {observers.push_back(o);}
    void Notify() override {for (Observer* observer : observers) observer->Update();}
private:
    string alias;
    int resistance;
    //new part for this lab:
    vector<Observer*> observers;
};

void Spy::identity() const{
    if (resistance == 0){
        Person::identity();
        cout << "My alias is: " << alias << endl;
    }
    else cout << "My name is: " << alias << endl;
}

void Spy::interrogate(){ 
    if (resistance > 0) {
        --resistance;
        if (resistance ==0) Notify(); 
    }
}

int main(int argc, char** argv) {
    Person agent("James Bond");
    Spy spy("Emilio Largo", "William Johnson", 3);
    Spy spy2("Ernst Blofield", "John Keats", 5);
    Judge judge("Judge Barnhill");
    spy.Attach(&judge);
    spy2.Attach(&judge);

	cout << endl << "Nice to meet you. ";
	agent.identity();

	for (int i = 0; i < 6; ++i) {
		std::cout << "Who are you?" << std::endl;
		spy.interrogate();
		spy.identity();
	}
	spy.set_identity("Bill Munny");
	spy.identity();

	cout << endl << "Nice to meet you. ";
	agent.identity();

	for (int i = 0; i < 6; ++i) {
		cout << "Who are you?" << endl;
		spy2.interrogate();
		spy2.identity();
	}
	return 0;
}
