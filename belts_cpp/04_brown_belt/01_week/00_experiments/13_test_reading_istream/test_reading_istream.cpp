#include <iostream>
#include <sstream>

using namespace std;

int main() {
    istringstream input("        \n   \n            word\n");

    // for (string line; getline(input, line);) {
    //     cout << "line: " << line << endl;
    // }

    char c;
    input >> c;
    cout << "character: " << c << endl;

    return 0;
}
