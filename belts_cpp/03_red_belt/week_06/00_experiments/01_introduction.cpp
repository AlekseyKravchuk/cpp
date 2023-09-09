#include <iostream>
#include <string>
#include <vector>

#include "../../log_duration.h"

// функция возвращает "тяжелую" строку
std::string MakeString() {
    return std::string(1'000'000'000, 'a');
}

// хотим добавить сгенерированную строку в вектор строк
void WithTempVariable() {
    std::vector<std::string> strings;
    std::string heavy_string = MakeString();
    strings.push_back(heavy_string);
}

// хотим добавить сгенерированную строку в вектор строк; первая оптимизация
void WITHOUT_TempVariable() {
    std::vector<std::string> strings;
    strings.push_back(MakeString());    
}

int main() {
    {
        LOG_DURATION("With temp variable");
        WithTempVariable();
        std::cout << "---------------" << std::endl;
    }

    {
        LOG_DURATION("WITHOUT temp variable");
        WITHOUT_TempVariable();
        std::cout << "---------------" << std::endl;
    }

    {
        LOG_DURATION("WITHOUT temp variable, using temp object created by Ctor");
        // Добавим в вектор временный объект, созданный не с помощью функции, а с помощью конструктора std::string
        std::vector<std::string> strings;
        strings.push_back(std::string(1'000'000'000, 'a'));
        std::cout << "---------------" << std::endl;
    }

    return 0;
}
