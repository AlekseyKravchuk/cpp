#include <algorithm>
#include <iostream>
#include <vector>

#include "print.h"

using namespace std;

int main() {
    vector<int> v{34, 12, 1, 7, 3, 2, 11, 70, 49, 8, 13, 22};
    vector<int> v_sorted(v.begin(), v.end());
    std::sort(v_sorted.begin(), v_sorted.end());

    cout << "v_sorted            : " << v_sorted << endl;


    auto middle_it = v.begin() + v.size() / 2;

    nth_element(v.begin(), middle_it, v.end());
    cout << "middle_it = " << *middle_it << endl;
    cout << "v_splitted_by_median: " << v << endl;

    return 0;
}
