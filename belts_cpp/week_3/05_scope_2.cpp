#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

int main() {
    std::string s = "Hello!"s;
    {
        std::string s = "world"s;

        // в С++ во внутреннем блоке можно определять переменную с тем же именем, что и во внешнем блоке
        // компилятор берет ту переменную, которая объявлена ближе
        // это ПЛОХОЙ подход - так делать НЕ НУЖНО!!!
        std::cout << s << std::endl;
    }
    std::cout << s << std::endl;
    
    return 0;
}