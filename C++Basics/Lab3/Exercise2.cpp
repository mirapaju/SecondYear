#include <iostream>
#include <deque>
#include <numeric>

using namespace std;

class RunningAverage{
public:
    explicit RunningAverage(int size=10) : max_size(size) {} 
    void add_value(int value);
    double get_average();
private:
    int max_size;
    int elem_count=0; 
    deque<int> numbers;
};

void RunningAverage::add_value(int value){ 
    if (size(numbers) >= max_size){
        numbers.pop_front();
        --elem_count;
    }
    numbers.push_back(value);
    ++elem_count;
}

double RunningAverage::get_average(){
    if (numbers.empty()) return 0.0;
    else return accumulate(numbers.begin(), numbers.end(), 0.0) / elem_count;
}


int main() {
    RunningAverage avg(5);
    bool stop =false;

    int input;
    while (!stop) {
        cout << "Enter a number or enter 0 to stop:";
        cin >> input;

        if (input == 0) stop = true;
        else {
            avg.add_value(input);
            cout << "Current running average: " << avg.get_average() << endl;
        }
    }
    cout << "Final running average:" << avg.get_average() << endl;
    return 0;
}
