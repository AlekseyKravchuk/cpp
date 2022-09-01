#include <iostream>
#include <numeric>  // for std::reduce()
#include <vector>

using namespace std;

int main() {
    auto vec = std::vector<int>{1, 2, 3, 4, 5};
    auto average = reduce(vec.begin(), vec.end(), 0, std::plus()) / static_cast<double>(vec.size());

    cout << "average of vector {1, 2, 3, 4, 5}: "s << average << endl;

    return 0;
}
