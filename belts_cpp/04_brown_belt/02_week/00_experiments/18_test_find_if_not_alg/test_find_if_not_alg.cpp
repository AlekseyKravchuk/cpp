#include <algorithm>
#include <iostream>

#include "print.h"

using namespace std;

int main() {
    std::vector<int> numbers{5, 46, -5, -6, 23, 17, 5, 9, 6, 5};
    cout << numbers << endl;
    cout << "===============================================================" << endl;

    int border = 3;

    // возвращает первый итератор "it", удовлетворяющий условию "pred(*it) is false"
    auto it = find_if_not(numbers.begin(),
                          numbers.end(),
                          [border](int current) {
                              return current > border;
                          });
    if (it != numbers.end()) {
        cout << "*it = " << *it << ", i = " << it - numbers.begin() << endl;
    }
    
    auto it2 = find_if_not(++it,
                           numbers.end(),
                           [border](int current) {
                               return current > border;
                           });

    if (it2 != numbers.end()) {
        cout << "*it2 = " << *it2 << ", i = " << it2 - numbers.begin() << endl;
    }                           
    


    // size_t count = 0;
    // int val = 5;
    // auto it_start = numbers.begin();
    // auto it_end = numbers.end();

    // while ((it_start = std::find_if_not(it_start,
    //                                     it_end,
    //                                     [val](int n) {
    //                                         return n > val;
    //                                     })) != it_end) {
    //     ++count;
    //     ++it_start;
    // }

    // std::cout << count << " elements were found that are not greater than \"" << val << "\"" << std::endl;

    return 0;
}
