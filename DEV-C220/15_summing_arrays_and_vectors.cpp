#include <iostream>
#include <vector>

template <typename Collection>
auto sum(const Collection& collection) {
    // объявляем переменную "sumResult" и задаем ей нужный нам тип через "decltype"
    // и инициализируем её дефолтным значением

    // std::begin(collection) возвращает ИТЕРАТОР
    // оператор разыменования итератора "*std::begin(collection)" возвращает ССЫЛКУ
    // нас не устраивает результат применения операции разыменования итератора, поскольку
    // в выражении для decltype нам нужен ТОЧНО ТАКОЙ ЖЕ тип, как и тип элемента коллекции
    // для этого нужно придумать (досочинить) соответствующее КОРРЕКТНОЕ с точки зрения компилятора выражение
    // (при этом вычисляться оно не будет)
    // преобразуем исходное выражение "decltype(*std::begin(collection))", чтобы в выражении возвращалась не ссылка, а ЗНАЧЕНИЕ:
    // "operator+" (ровно, как и "operator-") возвращает ПО ЗНАЧЕНИЮ => его можно использовать, чтобы сгенерировать нужное ВЫРАЖЕНИЕ
    // результат суммы двух ссылок на элементы коллекции вернет ОБЪЕКТ нужного нам типа (по значению)
    decltype(*std::begin(collection) + *std::begin(collection)) sumResult{};

    // ТАК НЕЛЬЗЯ:
    // decltype(*std::begin(collection)) sumResult{};  // error: assignment of read-only reference ‘sumResult’

    for (const auto& elm : collection) {
        sumResult = sumResult + elm;
    }

    return sumResult;
}

int main() {
    using namespace std::literals;

    std::vector<int> v = {1, 2, 3};
    int ar[] = {10, 20, 30};

    std::cout << sum(v) << std::endl;
    // std::cout << sum(ar) << std::endl;

    return 0;
}
