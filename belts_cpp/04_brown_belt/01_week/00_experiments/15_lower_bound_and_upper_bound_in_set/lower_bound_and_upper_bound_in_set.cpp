#include "print.h"

using namespace std;

int main() {
    // {1, 3, 5, 5, 5, 5, 6, 6, 8, 9, 9, 11, 14, 21, 22}
    multiset<int> my_set = {3, 5, 1, 6, 9, 11, 22, 14, 5, 9, 21, 5, 8, 6, 5};
    // int value = 5;
    // int value = 11;
    int value = 12;
    auto lb = my_set.lower_bound(value);
    auto ub = my_set.upper_bound(value);

    cout << my_set << endl;
    cout << "The number of \"" << value << "\" in multiset: " << distance(lb, ub) << " items." << endl;

    return 0;
}
