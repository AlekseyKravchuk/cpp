#include <iostream>
#include <typeinfo>
#include <iomanip>

class A {
   private:
    int x{};
};

class B {
   private:
    int y{};
    int z{};
};

int main() {
    const std::type_info& t_info = typeid(int);
    std::cout << std::boolalpha << t_info.__is_function_p() << std::endl;
    std::cout << std::boolalpha << t_info.name() << std::endl;
    std::cout << std::boolalpha << (t_info == typeid(int)) << std::endl;

    // std::cout << typeid(int).name() << std::endl;
    // std::cout << typeid(A).name() << std::endl;
    // std::cout << typeid(B).name() << std::endl;
    // std::cout << (typeid(int) == typeid(int)) << std::endl;
    return 0;
}
