// C++ program to illustrate map::insert({key, element})
#include <iomanip>
#include <iostream>
#include <map>

#include "../../print.h"

int main() {
    std::map<int, int> dict;  // initialize container

    // insert elements in random order
    dict.insert({2, 30});
    dict.insert({1, 40});
    dict.insert({3, 60});

    // std::map::insert возвращает пару: 1-ый элемент пары - это ИТЕРАТОР на вставленную пару {KEY, VALUE}
    //                                   2-ой элемент пары - это БУЛЕВО значение, если "true", то std:pair{KEY, VALUE} был успешно вставлен
    auto p = dict.insert({100, 555});
    dict.insert({200, 666});

    std::cout << dict << std::endl;
    std::cout << "*p.first = " << *p.first << std::endl;
    std::cout << "p.second = " << std::boolalpha << p.second << std::endl;

    return 0;
}
