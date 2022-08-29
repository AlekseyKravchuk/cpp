#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

int main() {
    vector<int> v1 = {10, 15, 1, 4, 22, 7, 11, 9, 6, 3, 14, 8, 31};
    set<int> s1(v1.begin(), v1.end());

    vector<int> v2 = {10, 15, 1, 4, 22, 7, 11, 9, 6, 3, 82, 90};
    set<int> s2(v2.begin(), v2.end());

    std::set<int> result;

    std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(),
                        std::inserter(result, result.end()));

    for (auto elm : result) {
        cout << elm << " ";
    }
    cout << endl;

    return 0;
}
