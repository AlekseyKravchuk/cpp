// istringstream::str
#include <iostream> // std::cout
#include <sstream>  // std::istringstream
#include <string>   // std::string

using namespace std;

int main() {
    istringstream iss;
    string strvalues = "32 240 2 1450";

    iss.str(strvalues);

    int val;
    for (int n = 0; n < 4; n++) {
        iss >> val;
        cout << val << endl;
    }
    std::cout << "Content of istringstream: " << iss.str() << endl;

    return 0;
}