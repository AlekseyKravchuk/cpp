#include <functional>  // std::bind
#include <iomanip>
#include <iostream>

using namespace std;

// C-style function
bool my_greater(int x, int y) {
    return x > y;
}

struct Equal {
    Equal() {
        cout << "Equal::Equal() called " << endl;
    }

    Equal(const Equal&) {
        cout << "Equal copy constructor called " << endl;
    }

    Equal(Equal&&) {
        cout << "Equal MOVE constructor called " << endl;
    }

    bool operator()(int x, int y) {
        return x == y;
    }
};

struct S {
    bool not_equal(int x, int y) const {
        return x != y;
    }
};

int main() {
    // хотим превратить эту функцию с 2-мя аргументами в функцию с одним аргументом, зафиксировав некий параметр
    std::function<bool(int, int)> compare = [](int x, int y) -> bool {
        return x < y;
    };

    // "f" is function object
    // хотим превратить "f" в фунцию с одним параметром путем фиксации одного из параметров
    // первым параметром идет функция, а далее - те параметры, которые хочется зафиксировать с инициализирующими значениями
    // std::placeholders::_1 - это специальная константа, объявленная в <functional>
    // в качестве placeholders мы указываем те аргументы, которые НЕ хотим фиксировать

    // фиксируем второй аргумент (равен нулю)
    auto compare_less_than_zero = std::bind(compare, std::placeholders::_1, 0);

    // фиксируем перый аргумент (равен 100)
    auto compare_greater_than_100 = std::bind(compare, 100, std::placeholders::_1);

    cout << boolalpha << "compare(10, 20): " << compare(10, 20) << endl;
    cout << boolalpha << "compare_less_than_zero(-5): " << compare_less_than_zero(-5) << endl;
    cout << boolalpha << "compare_greater_than_100(255): " <<  compare_greater_than_100(255) << endl;

    // cout << boolalpha << f(10, 100) << endl;

    // // ================================================================================

    // // f = my_greater;  // вместо указателя на функцию можно просто использовать ее имя
    // f = &my_greater;
    // cout << boolalpha << f(10, 100) << endl;

    // // ================================================================================
    // f = std::move(Equal());
    // cout << boolalpha << f(10, 100) << endl;
    // // ================================================================================
    // std::function<bool(const S&, int, int)> f2 = &S::not_equal;
    // S s_obj;
    // cout << boolalpha << f2(s_obj, 10, 100) << endl;

    return 0;
}
