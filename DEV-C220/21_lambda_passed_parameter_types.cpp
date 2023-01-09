// Требуется увеличить все элементы контейнера на 100 с помощью std::for_each и лямбда-функции
// ПРИМЕЧАНИЕ: std::for_each является НЕМОДИФИЦИРУЮЩИМ алгоритмом, а здесь используется только в качестве примера

#include <algorithm>  // std::for_each
#include <iterator>   // std::begin(), std::end(), std::back_inserter
#include <vector>

#include "my_print.h"

int main() {
    std::vector<int> v1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> v2 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // в первом случае передаем параметр в лямбду ПО ЗНАЧЕНИЮ, поэтому в лямбде искользуется КОПИЯ аргумента  
    std::for_each(v1.begin(), v1.end(),
                  [](int val) {
                      val += 100;
                  });
    std::cout << "v1 = ";
    PrintCollection(v1);

    // во втором случае передаем параметр в лямбду ПО ССЫЛКЕ, поэтому в лямбду передается АДРЕС аргумента
    // соответственно, по полученному лямбдой адресу можно модифицировать значение  
    std::for_each(v2.begin(), v2.end(),
                  [](int& val) {
                      val += 100;
                  });
    std::cout << "v2 = ";
    PrintCollection(v2);

    return 0;
}
