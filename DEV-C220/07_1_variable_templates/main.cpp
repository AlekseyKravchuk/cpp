#include <iostream>

#include "temp.h"

int main() {
    A<int> a1;
    A<int> a2;

    // к статическим членам можно получать доступ 2-мя способами:
    // 1) посредством объекта (ссылки или указателя)
    // 2) посредством класса через указания оператора разрешения области видимости
    // std::cout << A<int>::GetCount() << std::endl;

    // также статический метод GetCount() можно вызвать посредством объекта
    std::cout << a1.GetCount() << std::endl;

    A<double> a3;
    std::cout << a3.GetCount() << std::endl;

    return 0;
}
