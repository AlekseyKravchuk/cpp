#include <iostream>
#include <string>
#include <utility> // std::move

// в качестве параметра функция "test" принимает rvalue-ссылку
void test(std::string&& data) {
    data += " in function!";
    std::cout << data << std::endl;
}

int main() {
    test(std::string("first test")); // OK

    std::string str("second test");

    /*
        // ERROR: cannot bind rvalue reference of type ‘std::string&&’ to lvalue of type ‘std::string’
        //
        test(str);
    */

    test(std::move(str));
    std::cout << "Content of \"str\" variable after moving from it: " << str << std::endl;

    return 0;
}
