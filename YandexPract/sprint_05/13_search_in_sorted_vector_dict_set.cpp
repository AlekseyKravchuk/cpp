#include <algorithm>
#include <iostream>
#include <set>
#include <string>

using namespace std;

set<int>::const_iterator FindNearestElement(const set<int>& numbers, int border) {
    if (numbers.empty()) {
        return numbers.end();
    }

    set<int>::const_iterator minElmIt = numbers.begin();
    set<int>::const_iterator maxElmIt = prev(numbers.end());

    if (border <= *minElmIt) {
        return minElmIt;
    } else if (border >= *maxElmIt) {
        return maxElmIt;
    } else {
        // (*minElmIt < border < *maxElmIt), возможны 2 варианта:

        // 1) число "border" присутствует среди элементов множества => возвращаем результат std::set::lower_bound
        if (numbers.count(border)) {
            return numbers.lower_bound(border);
        } else {  // 2) числа "border" НЕТ среди элементов множества
            auto it = numbers.lower_bound(border);
            auto diffBordePrev = abs(border - *(prev(it)));
            auto diffBorderCurrent = abs(border - *(it));

            if (diffBordePrev <= diffBorderCurrent) {
                return prev(it);
            } else {
                return it;
            }
        }
    }

    return numbers.end();
}

int main() {
    set<int> numbers = {1, 4, 6};
    // auto res = *FindNearestElement(numbers, 3);
    // auto res = *FindNearestElement(numbers, 5);

    cout << *FindNearestElement(numbers, 0) << " " << *FindNearestElement(numbers, 3) << " "
         << *FindNearestElement(numbers, 5) << " " << *FindNearestElement(numbers, 6) << " "
         << *FindNearestElement(numbers, 100) << endl;
    set<int> empty_set;
    cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << endl;
    return 0;
}
