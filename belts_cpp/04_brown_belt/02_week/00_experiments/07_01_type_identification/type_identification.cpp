#include <iostream>
#include <typeinfo>
#include <iomanip>

class A {
   private:
    int x{};
};

class B : public A {
   private:
    int y{};
    int z{};
};

class C : public A {
   private:
    int j{};
    int k{};
};

int main() {
    // // класс "std::type_info" предназначен для хранения информации о типе объекта
    // // для любого типа в еденственом экземпляре компилятор создает ОБЪЕКТ КЛАССА "type_info"
    // const std::type_info& t_info = typeid(int);
    // std::cout << std::boolalpha << t_info.__is_function_p() << std::endl;
    // std::cout << std::boolalpha << t_info.name() << std::endl;
    // std::cout << std::boolalpha << (t_info == typeid(int)) << std::endl;

    const std::type_info& t_info_B = typeid(B);
    std::cout << "typeid(B).name() =  " << t_info_B.name() << std::endl;
    std::cout << std::boolalpha << t_info_B.before(typeid(A)) << std::endl;
    std::cout << std::boolalpha << t_info_B.before(typeid(C)) << std::endl;

    const std::type_info& t_info_A = typeid(A);
    std::cout << "typeid(A).name() =  " << t_info_A.name() << std::endl;
    std::cout << std::boolalpha << t_info_A.before((typeid(C))) << std::endl;

    enum class COLOR {
        RED,
        GREEN,
        BLUE
    };
    std::cout << "typeid(COLOR).name() =  " << typeid(COLOR).name() << std::endl;

    // std::cout << typeid(int).name() << std::endl;
    // std::cout << typeid(A).name() << std::endl;
    // std::cout << typeid(B).name() << std::endl;
    // std::cout << (typeid(int) == typeid(int)) << std::endl;

    return 0;
}
