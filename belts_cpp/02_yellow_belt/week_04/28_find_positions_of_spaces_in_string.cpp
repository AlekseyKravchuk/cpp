#include <algorithm>  // std::find
#include <iostream>
#include <string>

using namespace std::literals;

int main() {
    std::string s = "one two three and some word"s;
    for (auto it = std::find(s.begin(), s.end(), ' ');
         it != s.end();
         it = std::find(std::next(it), s.end(), ' ')) {
        std::cout << it - s.begin() << " ";
    }
    std::cout << std::endl;

    return 0;
}
