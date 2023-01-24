#include "my_unique_ptr.h"

#include <iostream>
#include <string>
#include <utility>  // std::move

int main() {
    using namespace std::literals;

    MyUniquePTR<std::string> p1(new std::string("abc"s));
    std::cout << p1->c_str() << std::endl;

    // std::string s2 = *p1;
    std::string s2(*p1);
    std::cout << "s2 = "s << s2 << std::endl;

    // error: use of deleted function ‘MyUniquePTR<T>::MyUniquePTR(const MyUniquePTR<T>&)
    // MyUniquePTR<std::string> p2 = p1;

    MyUniquePTR<std::string> p2 = std::move(p1);

    if (!p1) {
        std::cout << "No object!"s << std::endl;
    }

    return 0;
}
