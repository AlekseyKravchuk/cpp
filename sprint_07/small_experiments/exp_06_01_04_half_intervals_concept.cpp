#include <algorithm>
#include <iostream>
#include <ostream>
#include <vector>

using namespace std;

template <typename Iterator>
void PrintRange(Iterator range_begin,
                Iterator range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        if (std::next(it) != range_end) {
            // std::cout << *it << " "s;
            std::cout << *it << ", "s;
        } else {
            // std::cout << *it << std::endl;
            std::cout << *it << std::endl;
        }
    }
}

int main() {
    vector<string> numbers = {"one"s, "two"s, "three"s, "four"s, "five"s, "six"s, "seven"s};

    // хотим вывести два интервала: 1) [1; 3], 2) [4, 7]
    vector<string>::iterator it_border = std::find_if(numbers.begin(),
                                                      numbers.end(),
                                                      [](const string& s) {
                                                          return s.substr(0, 2) == "fo"s;
                                                      });
    PrintRange(numbers.begin(), it_border);
    PrintRange(it_border, numbers.end());

    return 0;
}
