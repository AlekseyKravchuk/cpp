// string::begin/end
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    std::string str = "Test string for some purpose"s;

    for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
        std::cout << *it;
    }
    cout << endl;

    std::string::iterator it = std::find(str.begin(), str.end(), ' ');

    return 0;
}