// Требуется с помощью std::for_each распечатать куб каждого элемента.
// Предикат должен быть задан посредством функтора

#include <algorithm>  // std::for_each
#include <cmath>      // std::pow
#include <iterator>   // std::begin, std::end

#include "my_print.h"

class PrintCube {
   public:
    void operator()(int x) const {
        std::cout << std::pow(x, 3) << " ";
    }
};

int main() {
    int ar[] = {5, 9, -6, -10};

    // третьим параметров в "std::for_each" передается объект типа "Pred", проинициализированный конструктором по умолчанию
    std::for_each(std::begin(ar), std::end(ar), PrintCube());
    std::cout << std::endl;

    // того же результата можно было добиться с помощью лямбда-функции:
    std::for_each(std::begin(ar), std::end(ar),
                  [](int x) {
                      std::cout << std::pow(x, 3) << " ";
                  });
    std::cout << std::endl;

    return 0;
}
