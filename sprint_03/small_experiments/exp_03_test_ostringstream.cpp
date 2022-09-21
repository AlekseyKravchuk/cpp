#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    int example = 228;
    std::ostringstream oss;

    oss << example;

    string example_as_string = oss.str();

    cout << example_as_string + " + another string" << endl; 

    return 0;
}