#include <cstdint>
#include <iostream>
#include <limits>

using namespace std;

int main() {
    int x = -1;
    unsigned y = 1;
    // cout << (x < y) << endl;
    cout << static_cast<unsigned int>(x) << endl;
}