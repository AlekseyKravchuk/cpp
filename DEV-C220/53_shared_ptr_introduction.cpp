#include <iostream>
#include <memory>  // std::shared_ptr
#include <string>

using namespace std::literals;

int main() {
    // std::shared_ptr<std::string> sh1(new std::string("A"s));  // count = 1

    // более предпочтительный способ создания "std::shared_ptr", когда не нужен пользовательский Deleter
    std::shared_ptr<std::string> sh1 = std::make_shared<std::string>("A"s);
    std::cout << "sh1 users: "s << sh1.use_count() << std::endl;

    std::shared_ptr<std::string> sh2(new std::string("B"s));  // count = 1
    std::cout << "sh2 users: "s << sh2.use_count() << std::endl;

    std::shared_ptr<std::string> sh3 = sh2;
    std::cout << "sh3 users: "s << sh3.use_count() << std::endl;

    sh3 = sh1;
    std::cout << "sh3 users after \"sh3 = sh1;\": "s << sh3.use_count() << std::endl;
    std::cout << "sh1 users after \"sh3 = sh1;\": "s << sh1.use_count() << std::endl;

    return 0;
}
