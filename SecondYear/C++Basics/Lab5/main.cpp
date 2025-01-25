#include <iostream>
#include <ostream>
#include <vector>
#include <algorithm>
#include <iterator>

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
    int number;
    do {
        number = rand() % (max - min + 1) + min;
    } while(find(numbers.begin(), numbers.end(), number) != numbers.end());
    numbers.push_back(number);
    return number;
}

class Print {
public:
    explicit Print(ostream& out) : out(out), index(1) {}

    void operator()(int num) {
        out << "#" << index++ << ": " << num << endl;
    }

private:
    ostream& out;
    int index;
};

int main() {
    string response ="y";

    while (response == "y") {
        vector<int> lotto(7);
        vector<int> v_lotto(6);
        vector<int> e_lotto(5);
        //  test sets:
        /*  vector<int> lotto = {1,2,3,4,5,6,7};
           vector<int> v_lotto = {4,5,6,7,8,9};
           vector<int> e_lotto ={1,5,6,8,9};*/
        RandGen_2 l(1, 40);
        RandGen_2 v(1, 48);
        RandGen_2 e(1, 50);

        generate(lotto.begin(), lotto.end(), l);    //fill the vectors with random numbers of specific range
        generate(v_lotto.begin(), v_lotto.end(), v);
        generate(e_lotto.begin(), e_lotto.end(), e);

        //print numbers of first two sets
        cout << "Lotto: ";
        copy(lotto.begin(), lotto.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
        cout << "Viking lotto: ";
        copy(v_lotto.begin(), v_lotto.end(), ostream_iterator<int>(cout, " "));
        cout << endl;

        //find the common numbers of first two sets and print them with index
        vector<int> match;
        Print print(cout);
        cout << "Matching numbers: " << endl;
        sort(lotto.begin(), lotto.end());
        sort(v_lotto.begin(), v_lotto.end());
        set_intersection(lotto.begin(), lotto.end(), v_lotto.begin(), v_lotto.end(),
                         back_inserter(match));
        for_each(match.begin(), match.end(), print);


        //print the numbers of last set
        cout << "Eurojackpot: ";
        copy(e_lotto.begin(), e_lotto.end(), ostream_iterator<int>(cout, " "));
        cout << endl;

        //find the common numbers of all the sets, print with index
        vector<int> match2;
        sort(e_lotto.begin(), e_lotto.end());
        set_intersection(match.begin(), match.end(), e_lotto.begin(), e_lotto.end(),
                         back_inserter(match2));
        cout << "Matching numbers in three sets: " <<endl;
        for_each(match2.begin(), match2.end(), print);

        cout << "Do you wish to continue (y/n)?";
        cin >> response;
        cout << endl;
    }
    cout << "Program stopped." << endl;
}


