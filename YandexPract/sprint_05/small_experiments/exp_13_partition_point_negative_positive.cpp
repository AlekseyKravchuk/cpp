// C++ program to get negative elements
// and positive elements using partition_point
#include <algorithm>  // std::partition, std::partition_point
#include <iostream>   // std::cout
#include <vector>     // std::vector

bool IsNegative(int i) { return (i < 0); }

int main() {
    std::vector<int> data{1, -1, 3, -4, 5, 2, -2, 4, -5, -3};
    std::vector<int> negative, positive;

    // partition data on the basis of odd elements using pred IsNegative()
    std::stable_partition(data.begin(), data.end(), IsNegative);

    // gets the partition point based on odd elements
    auto it = std::partition_point(data.begin(), data.end(), IsNegative);

    // assign elements to odd from beginning till partition point.
    negative.assign(data.begin(), it);
    positive.assign(it, data.end());

    // print contents of odd:
    std::cout << "Negative: " << std::endl;
    for (int& x : negative)
        std::cout << '\t' << x << '\n';

    // print contents of even:
    std::cout << "Positive:" << std::endl;;
    for (int& x : positive)
        std::cout << '\t' << x << '\n';

    return 0;
}
