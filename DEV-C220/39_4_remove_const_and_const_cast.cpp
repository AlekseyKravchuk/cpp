#include <iostream>
#include <ostream>
#include <string>
#include <type_traits>
#include <vector>

using namespace std::literals;

// "const_cast" - это изменение свойств сущности (существующего программного объекта)
// "const_cast" снимает константность с УКАЗЫВАЕМОГО значения, а "std::remove_const" - это другой тип
int main() {
    char a = 'a';
    const char* p2 = &a;
    // ++(*p2); // а изменять указываемое значение нельзя
    ++p2; // указатель двигать можно

    // auto p3 = const_cast<char*>(p2);
    // (*p3)++;

    // (*const_cast<char*>(p2))++;
    // const_cast<> - это изменение сущности "p2"
    std::cout << (*const_cast<char*>(p2))++ << std::endl;

    const char* p3 = "hello";

    // "std::remove_const" используется для задания нового типа на базе уже имеющегося
    std::remove_const_t<decltype(p3)> p4 = nullptr;
    char* p5 = nullptr;
    p4 = p5;

    return 0;
}
