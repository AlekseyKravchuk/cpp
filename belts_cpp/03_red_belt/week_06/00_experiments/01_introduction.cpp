#include <iostream>
#include <string>
#include <vector>

#include "../../log_duration.h"

// функция возвращает "тяжелую" строку
std::string MakeString() {
    return std::string(1'000'000'000, 'a');
}

// хотим добавить сгенерированную строку в вектор строк
void NonOptimizedFunc() {
    std::vector<std::string> strings;
    std::string heavy_string = MakeString();
    strings.push_back(heavy_string);
}

// хотим добавить сгенерированную строку в вектор строк; первая оптимизация
void FirstlyOptimizedFunc() {
    std::vector<std::string> strings;
    strings.push_back(MakeString());
}

int main() {
    {
        LOG_DURATION("NonOptimizedFunc");
        NonOptimizedFunc();
    }

    {
        LOG_DURATION("FirstlyOptimizedFunc");
        FirstlyOptimizedFunc();
    }

    return 0;
}
