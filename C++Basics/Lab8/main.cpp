#include <iostream>

using namespace std;

class Counter {
public:
	virtual void inc() = 0;
	virtual void dec() = 0;
	virtual operator int() = 0;
	virtual ~Counter() {};
};

//constructor takes two parameters: initial value and upper limit
class LimitedCounter: public Counter{
public:
    LimitedCounter(int initial, int upper) : value(initial), limit(upper) {}
    void inc() override {if (value < limit) ++value;} //can’t be incremented over the upper limit, if value reached the upper limit nothing happens
    void dec() override {if (value > 0) --value;} //can’t be decremented below zero, if counter is zero then calling dec() will have no effect
    operator int() override {return value;}
private:
    int value;
    int limit;
};

//constructor takes two parameters: initial value and upper limit
class OverflowCounter : public Counter{
public:
    OverflowCounter(int initial, int upper) : value(initial), limit(upper) {}
    void inc() override;
    void dec() override; //when counter is zero decrementing the counter sets counter to upper limit.
    operator int() override {return value;}
private:
    int value;
    int limit;
};

//when counter has reached the upper limit incrementing the value will set the counter to zero
void OverflowCounter::inc(){
    if (value==limit) value = 0;
    else ++value;
}

//when counter has reached zero incrementing the value will set the counter to limit
void OverflowCounter::dec() {
    if (value==0) value = limit;
    else --value;
}

// A reference to Counter and number that tells how
//many times the counter should be incremented or decremented. A negative value
//decrements counter and positive value increments counter.
void UseCounter(Counter& ctr, int num){
    if (num>0) {
        for (int i = 0; i < num; i++) ctr.inc();
    }
    else {
        for (int i = 0; i < abs(num); i++) ctr.dec();
    }
};


int main(int argc, char** argv) {
	LimitedCounter lc(0, 5);
	OverflowCounter oc(5, 9);

	cout << oc << endl;
	UseCounter(oc, 5);
	cout << oc << endl; // should display zero
	UseCounter(oc, -1);
	cout << oc << endl; // should display 9
	oc.dec();
	cout << oc << endl; // should display 8

	cout << lc << endl; //should display 0 (initial) ?
	lc.inc(); 
	cout << lc << endl; //should display 1?
	lc.dec(); 
	cout << lc << endl; //should display 0
	for(int i = 0; i < 10; ++i) lc.inc(); //try to ++ 10 times, no wrapping so max is 5
	cout << lc << endl; //so should display 5
	UseCounter(lc, -9); //decrement 9, wrapping so
	cout << lc << endl; //should display 0


	return 0;
}
