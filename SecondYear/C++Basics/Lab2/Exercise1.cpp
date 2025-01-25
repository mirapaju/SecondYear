#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>

using namespace std;

int main() {
    bool stop =false;
    while (!stop) {
        string input;
        cout << "Enter a line with numbers or \"stop\":";
        getline(cin, input);

        if (input.substr(0, 4) == "stop") stop = true;
        else {
            istringstream in_stream(input);
            vector<int> nums;
            int number;

            while (in_stream >> number) nums.push_back(number);
            cout << "Total of " << nums.size() << " numbers is " << accumulate(nums.begin(), nums.end(), 0) << endl;
        }
    }
    cout << "Program ended." << endl;
    return 0;
}