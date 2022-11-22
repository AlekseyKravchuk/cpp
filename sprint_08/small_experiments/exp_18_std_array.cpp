#include <array>
#include <iostream>
#include <string>

using namespace std;

int main() {
    std::array<int, 10> ar;
    ar[0] = 15;
    ar[1] = 25;
    std::cout << ar[0] << std::endl;
    std::cout << ar[1] << std::endl;

    return 0;
}
