#include <algorithm>
#include <iostream>
#include <set>
#include <string>

using namespace std;

int main() {
    // // set<int> mySet = {4, 5, 7, 9, 12, 12, 12, 15};
    // set<int> mySet = {10, 20, 30, 40, 50, 60};
    // set<int> mySet = {2, 3, 4, 6, 7};
    // set<int> mySet = {4, 6};
    set<int> mySet = {4, 5, 6};

    int boundLower = 5;
    auto itLower = mySet.lower_bound(boundLower);
    if (itLower != mySet.end()) {
        cout << "The lower bound of key \"" << boundLower << "\" is " << (*itLower) << endl;
        cout << "Position of returned iterator is: " << std::distance(mySet.begin(), itLower) << endl;
    } else {
        cout << "The element \"" << boundLower << "\" is larger than the greatest element in the set" << endl;
    }
    cout << endl
         << "*********************" << endl;

    int boundUpper = 5;
    auto itUpper = mySet.upper_bound(boundUpper);
    if (itUpper != mySet.end()) {
        cout << "The upper bound of key \"" << boundUpper << "\" is " << (*itUpper) << endl;
        cout << "Position of returned iterator is: " << std::distance(mySet.begin(), itUpper) << endl;
    } else {
        cout << "The element \"" << boundUpper << "\" is larger than the greatest element in the set" << endl;
    }
    cout << endl
         << "*********************" << endl;

    int bound = 5;
    auto [lower, upper] = mySet.equal_range(bound);
    if (lower != mySet.end() && upper != mySet.end()) {
        cout << "The RANGE for key \"" << bound << "\" is [" << (*lower) << "; "s << (*upper) << "]"s << endl;
        cout << "Position range of returned pair of iterators is: ["
             << std::distance(mySet.begin(), lower) << "; "s << std::distance(mySet.begin(), upper) << "]"s << endl;
    } else {
        cout << "The element \"" << boundUpper << "\" is larger than the greatest element in the set" << endl;
    }
    cout << endl
         << "*********************" << endl;

    return 0;
}
