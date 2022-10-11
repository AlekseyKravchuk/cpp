#include <algorithm>
#include <iostream>
#include <set>
#include <string>

using namespace std;

int main() {
    // set<int> mySet = {4, 5, 7, 9, 12, 12, 12, 15};
    set<int> mySet = {10, 10, 20, 30, 40, 40, 40, 50, 60};

    int boundLower = 1;
    auto it = mySet.lower_bound(boundLower);
    if (it != mySet.end()) {
        cout << "The lower bound of key \"" << boundLower << "\" is " << (*it) << endl;
        cout << "Position of returned iterator is: " << std::distance(mySet.begin(), it) << endl;
    } else {
        cout << "The element \"" << boundLower << "\" is larger than the greatest element in the set" << endl;
    }

    cout << endl << "*********************" << endl;

    int boundUpper = 15;
    auto itUpper = mySet.upper_bound(boundUpper);
    if (itUpper != mySet.end()) {
        cout << "The upper bound of key \"" << boundUpper << "\" is " << (*itUpper) << endl;
        cout << "Position of returned iterator is: " << std::distance(mySet.begin(), itUpper) << endl;
    } else {
        cout << "The element \"" << boundUpper << "\" is larger than the greatest element in the set" << endl;
    }

    return 0;
}
