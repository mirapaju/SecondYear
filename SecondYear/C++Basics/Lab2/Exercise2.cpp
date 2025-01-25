#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>

using namespace std;
namespace fs = std::filesystem;

int main() {
    vector<string> dir;
    string input;

    cout << "Enter directory names or a blank line to stop:";
    while (getline(cin, input) && !input.empty()) dir.push_back(input);

    if (!dir.empty()){
        fs::path rel_p;
        for (const auto &path: dir) rel_p /= path; //create relative path from entered directory names

        fs::create_directories(rel_p);

        fs::path file_path = rel_p / "result.txt"; //path to new file
        ofstream file_output(file_path); //open file (creates the file if (and when) doesn't exist)
        fs::path absolute_path = fs::absolute(file_path); //create absolute path

        cout << "Absolute path to the file: " << absolute_path << endl;
        file_output << "Absolute path to the file: " << absolute_path << endl;

        cout << "Content of the string vector:" <<endl;
        for (const auto &directory: dir) {
            cout << directory << endl;
            file_output << directory << endl;
        }

        cout << "Relative path to the file: " << rel_p << endl;
        file_output << "Relative path to the file: " << rel_p << endl;
    }
    cout << "Program ended." << endl;
    return 0;
}