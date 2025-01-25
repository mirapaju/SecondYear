#include <iostream>
#include <ostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <exception>

using namespace std;

class RandGen_2 {
public:
    explicit RandGen_2(int min=0, int max=50): min(min), max(max), numbers() {srand(time(NULL));}
    int operator()();
private:
    int min;
    int max;
    vector<int> numbers;
};

int RandGen_2::operator()() {
    //NEW
    if (numbers.size() >= (max - min + 1)) {
        throw runtime_error("Unable to produce unique random number. ");
    }
    //
    int number;
    do {
        number = rand() % (max - min + 1) + min;
    } while(find(numbers.begin(), numbers.end(), number) != numbers.end());
    numbers.push_back(number);
    return number;
}


void test_generator(RandGen_2 ur, int count){
    vector<int> randoms; //push the generated random numbers here ->get the count of generated numbers if error is thrown during generation
    cout << "Generating numbers:" << endl;
    try {
        generate_n(back_inserter(randoms), count, ur);
        copy(randoms.begin(), randoms.end(), ostream_iterator<int>(cout, "\n"));
        cout << "End of generator" << endl;
    }
    catch (const runtime_error& e) {
        cout << "Exception: " << e.what() << "Tried to generate " << count << " random numbers. Got only " << randoms.size() << endl;
    }
};

int main(){
    RandGen_2 randGen(5, 13);
    test_generator(randGen, 6);
    test_generator(randGen, 9);
    test_generator(randGen, 13); // this will cause exception
    RandGen_2 randGen2(1, 35);
    test_generator(randGen2, 7);
    test_generator(randGen2, 7);
    test_generator(randGen2, 7);
    test_generator(randGen2, 70); // this will cause exception
}
