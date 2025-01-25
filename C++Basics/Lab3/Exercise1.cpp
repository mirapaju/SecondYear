#include <iostream>
#include <vector>
#include <map>

using namespace std;

int main() {
    bool stop =false;
    map<string,int> mymap;
    while (!stop) {
        string input;
        cout << R"(Enter name or "stop" or "print":)";
        cin >> input;

        if (input == "stop") stop = true;
        else if (input == "print") {
            if (mymap.empty()) cout << "Container empty." << endl;
            else for(auto & it : mymap) cout << it.first << " : " << it.second << endl;
        }
        else {
            cout << "Enter integer:";
            int num;
            cin >> num;
            mymap[input] = num;
        }
    }
    cout << "Program ended." << endl;
    return 0;
}