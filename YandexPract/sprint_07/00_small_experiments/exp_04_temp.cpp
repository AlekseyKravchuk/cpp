#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

int main() {
    int value = 17;
    int* valuePtr = &value;

    if (bool deleteNow = *valuePtr % 2 == 0; deleteNow) {
        std::cout << "*valuePtr % 2 = 0" << std::endl;
    } else {
        std::cout << "Wrong" << std::endl;
    }
    return 0;
}
