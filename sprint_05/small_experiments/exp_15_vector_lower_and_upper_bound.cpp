#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <vector>

using namespace std;

template <typename T>
typename std::vector<T>::iterator
insert_sorted(std::vector<T>& vec, T const& item) {
    return vec.insert(std::upper_bound(vec.begin(), vec.end(), item),
                      item);
}

template <typename Iterator>
void PrintRange(Iterator beginIt, Iterator endIt) {
    for (auto it = beginIt; it != endIt; ++it) {
        if (next(it) != endIt) {
            cout << *it << " "s;
        } else {
            cout << *it << endl;
        }
    }
}

int main() {
    // vector<int> v = {1, 2, 5};
    vector<int> v = {8, 5, 1};
    int val2insert = 3;
    // insert_sorted(v, val2insert);

    auto pos = std::find_if(v.begin(), v.end(), [&](const auto& val) {
        return val < val2insert;
    });

    v.insert(pos, val2insert);

    PrintRange(v.begin(), v.end());

    return 0;
}
