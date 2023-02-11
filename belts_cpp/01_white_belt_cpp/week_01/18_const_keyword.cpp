#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

int main() {
    // // модификатор "const" распространяется И на контейнер И на элементы этого контейнера
    // const std::vector<std::string> w = {"hello"s};
    // // w[0][0] = 'H';  // ERROR
    // std::cout << w[0] << std::endl;
    std::string s = "abc"s;
    s += "de"s;
    std::cout << s << std::endl;

    return 0;
}
