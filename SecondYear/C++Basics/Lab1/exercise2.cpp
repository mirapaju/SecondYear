#include <iostream>
#include <algorithm>

using namespace std;
void replace_char(string &str, char ch);

int main() {
    bool stop =false;
    while (!stop){
        string str;
        cout << "Enter a string:";
        getline(cin, str);
        cout << "Enter character to replace or 'stop' to quit:";

        if (auto option = string{}; getline(cin, option) && option == "stop") stop = true;
        else replace_char(str, option.front());
    }
    cout << "Okay so you are just leaving, bye then." << endl;
    return 0;
}

void replace_char(string &str, char ch) {
    replace(str.begin(), str.end(), ch, '_');
    cout << "Result: " << str << endl;
}