#include <iostream>
#include <string>

using namespace std;

class Person {
public:
	explicit Person(const char* name = "nobody") : name(name){};
	virtual ~Person() = default;
	virtual void identity() const;
	virtual void interrogate(){};
private:
	string name;
};

void Person::identity() const{
    cout << "My name is: " << name << endl;
}

class Spy: public Person{
public:
    Spy(const char *name, const char *fake_name, int resistance)
    : Person(name), alias(fake_name), resistance(resistance) {};
    void set_identity(const char *fake_name) { alias = fake_name;}
    void identity() const override;
    void interrogate() override {if (resistance > 0) --resistance;};
private:
    string alias;
    int resistance;
};

void Spy::identity() const{
    if (resistance == 0) {
        Person::identity();
        cout << "My alias is: " << alias << endl;
    }
    else cout << "My name is: " << alias << endl;
}


int main(int argc, char** argv) {
	Person agent("James Bond");
    agent.identity();
	Spy spy("Emilio Largo", "William Johnson", 3);
	Spy spy2("Ernst Blofield", "John Keats", 5);

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