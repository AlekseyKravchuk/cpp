#include <iostream>
#include <string>

using namespace std::literals;

int main() {
    const std::string& s = "hello"s;

    // оператор присваивания в С++ выполняет полное глубокое копирование значения одной переменной в другую
    std::string t = s;
    t += ", world!"s;

    std::cout << "s: " << s << std::endl;
    std::cout << "t: " << t << std::endl;
    
    return 0;
}
