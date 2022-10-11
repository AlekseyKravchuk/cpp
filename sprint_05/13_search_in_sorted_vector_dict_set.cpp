#include <algorithm>
#include <iostream>
#include <set>
#include <string>

using namespace std;

set<int>::const_iterator FindNearestElement(const set<int>& numbers, int border) {
    set<int>::const_iterator minElmIt = numbers.begin();
    set<int>::const_iterator maxElmIt = prev(numbers.end());

    if (border < *minElmIt) {
        return minElmIt;
    } else if (border > *maxElmIt) {
        return maxElmIt;
    } else {
        
    }




        if (!numbers.empty()) {
            set<int>::const_iterator minElmIt = numbers.begin();
            set<int>::const_iterator maxElmIt = prev(numbers.end());
        }
    return numbers.lower_bound(border);
}

int main() {
    set<int> numbers = {1, 4, 6};
    cout << *FindNearestElement(numbers, 1000) << endl;
    // cout << *FindNearestElement(numbers, 0) << " " << *FindNearestElement(numbers, 3) << " "
    //      << *FindNearestElement(numbers, 5) << " " << *FindNearestElement(numbers, 6) << " "
    //      << *FindNearestElement(numbers, 100) << endl;
    // set<int> empty_set;
    // cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << endl;
    return 0;
}
