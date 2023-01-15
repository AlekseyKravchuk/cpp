#include <iostream>

#include "temp.h"

int main() {
    A<int> a1_int;
    A<int> a2_int;
    A<double> a1_double;
    A<int> a3_int;

    // к статическим членам можно получать доступ 2-мя способами:
    // 1) посредством объекта (ссылки или указателя)
    // 2) посредством класса через указания оператора разрешения области видимости
    // std::cout << A<int>::GetCount() << std::endl;

    // также статический метод GetCount() можно вызвать посредством объекта
    // std::cout << a1_int.GetCount() << std::endl;
    std::cout << A<int>::GetCount() << std::endl;

    
    std::cout << A<double>::GetCount() << std::endl;

    return 0;
}
