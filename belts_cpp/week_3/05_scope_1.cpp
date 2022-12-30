#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

int main() {
    // переменные в С++ видны только в том блоке операторных скобок, в котором они объявлены
    {
        // переменная в С++ видна после своего объявления и до конца блока операторных скобок, в котором она объявлена
        int x = 5;
        {
            std::cout << x << std::endl;
        }
        std::cout << x << std::endl;
    }  // здесь заканчивается область видимости переменной "x"

    // ниже следующая инструкция не скомпилируется
    // std::cout << x << std::endl;
    
    return 0;
}