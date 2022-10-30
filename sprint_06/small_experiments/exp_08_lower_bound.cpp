#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    bool isFirst = true;
    for (auto it = range_begin; it != range_end; ++it) {
        if (isFirst) {
            std::cout << *it;
            isFirst = false;
        } else {
            std::cout << " "s << *it;
        }
    }
    cout << endl;
}

int main() {
    std::vector<int> v{1, 2, 2, 3, 4, 4, 4, 4, 4, 5, 6, 7, 8, 9, 10, 11, 11, 11};
    std::cout << "Vector contains: ";
    PrintRange(v.begin(), v.end());

    int val = 4;
    // std::vector<int>::iterator it_lower_bound;
    std::vector<int>::iterator it_upper_bound;

    int numOfLessOrEqual = std::distance(v.begin(), std::upper_bound(v.begin(), v.end(), val));
    double ratioLessOrEqual = numOfLessOrEqual / static_cast<double>(v.size());

    std::cout << "The number of elements less or equal to \""s << val << "\" is equal to: "
              << numOfLessOrEqual << std::endl;

    std::cout << "Ratio of numbers less or equal to the given one: " << ratioLessOrEqual << std::endl;
    

    // it_lower_bound = std::lower_bound(v.begin(), v.end(), val);
    // it_upper_bound = std::upper_bound(v.begin(), v.end(), val);

    // std::cout << "\"it_lower_bound\" iterator for element \""s << val << "\" points to the position: ";
    // std::cout << std::distance(v.begin(), it_lower_bound) << " in vector "s << std::endl;

    // std::cout << "\"it_upper_bound\" iterator for element \""s << val << "\" points to the position: ";
    // std::cout << std::distance(v.begin(), it_upper_bound) << " in vector "s << std::endl;

    // std::cout << "distance between lower_bound and upper_bound = " << std::distance(it_lower_bound, it_upper_bound) << std::endl;

    return 0;
}
