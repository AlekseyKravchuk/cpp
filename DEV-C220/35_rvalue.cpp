#include <iostream>
#include <string>
#include <utility>  // std::move

#include "my_print.h"

using namespace std::literals;

int main() {
    int v = 20;
    int* p1 = &(++v);
    std::cout << "p1 = " << p1 << std::endl;
    std::cout << "*p1 = " << *p1 << std::endl;
    std::cout << "v = " << v << std::endl;

    // int* p2 = &(v++); // ERROR: lvalue required as unary ‘&’ operand
    // std::cout << "p1 = " << p2 << std::endl;
    // std::cout << "*p1 = " << *p2 << std::endl;
    // std::cout << "v = " << v << std::endl;

    std::cout << std::endl;
    int ar[] = {10, 12, 14};

    // результат оператора индексирования - это тоже "lvalue"
    ar[2] = 7;
    // PrintCollection(ar);

    return 0;
}
