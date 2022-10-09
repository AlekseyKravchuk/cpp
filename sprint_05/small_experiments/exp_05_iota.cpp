// C++ program to illustrate
// std :: iota
#include <iostream>  // std::cout
#include <numeric>   // std::iota
#include <vector>

// Driver code
int main() {
    std::vector<int> numbers(20);

    // Initialising starting value as 100
    int startingValue = -5;
    // double startingValue = 10.0;

    // std::iota(numbers, numbers + 10, st);
    std::iota(numbers.begin(), numbers.end(), startingValue);

    std::cout << "Elements are :";
    for (auto i : numbers)
        std::cout << ' ' << i;
    std::cout << '\n';

    return 0;
}
