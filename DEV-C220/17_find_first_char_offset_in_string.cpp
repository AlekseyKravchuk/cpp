#include <algorithm>  // std::find
#include <iostream>
#include <iterator>   // std::distance
#include <string>

using namespace std::literals;

int main() {
    // std::string s{"abcdefg"s};
    std::string s{"abcefg"s};

    int pos = std::distance(s.begin(),
                            std::find(s.begin(), s.end(), 'd'));

    std::cout << pos << std::endl;

    return 0;
}
