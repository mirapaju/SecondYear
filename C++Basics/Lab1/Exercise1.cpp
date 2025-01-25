#include <iostream>
#include <vector>

using namespace std;

int main() {
    cout << "How many numbers do you want to enter: " << endl;
    int count;
    cin >> count;
    vector<int> numbers;

    for (int i=0, num; i< count; i++){
        cout << "Enter nr[" << i + 1 << "]:";
        cin >> num;
        numbers.push_back(num);
    }

    bool first = true;
    cout << "You entered: ";
    for (auto &number : numbers) {
        cout << (first ? "" : ", ") << number;
        first = false;
    }
    cout << endl;

    return 0;
}
