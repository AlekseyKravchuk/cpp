#include <iostream>
#include <iterator>
#include <vector>

#include "print.h"

using namespace std;

template <typename FwdIt, typename T>
FwdIt find(FwdIt begin, FwdIt end, const T& value) {
    for (; begin != end; ++begin) {
        if (*(begin) == value) {
            break;
        }
    }

    return begin;
}

int main() {
    vector<int> v = {11, 2, 14, 6, 18, 5, 77};
    int value_to_search = 5;

    // функция "find" будет вызвана для НЕконстантных итераторов
    auto it = find< vector<int>::const_iterator >(v.begin(), v.end(), value_to_search);
    if (it != v.end()) {
        cout << "value = " << *it << " was found in vector " << v << endl;
    }
    
    return 0;
}
