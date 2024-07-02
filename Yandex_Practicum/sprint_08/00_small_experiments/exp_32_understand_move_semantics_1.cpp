#include <cassert>
#include <iostream>
#include <list>
#include <numeric>
#include <ostream>
#include <vector>

using namespace std;

struct NonCopyableInt {
    int value;

    // обычный конструктор с параметром
    NonCopyableInt(int num) : value(num) {}

    // сообщаем компилятору, что конструктора копирования у класса не существует.
    // NonCopyableInt(const NonCopyableInt&) = delete;

    // добавили конструктор копирования в явном виде
    NonCopyableInt(const NonCopyableInt& other) {
        value = other.value;
    }

    // добавили оператор присваивания в явном виде
    NonCopyableInt& operator=(const NonCopyableInt& other) {
        value = other.value;

        return *this;
    }

    // сообщаем компилятору, что оператора присваивания у класса не существует.
    // NonCopyableInt& operator=(const NonCopyableInt&) = delete;

    // конструктор перемещения
    NonCopyableInt(NonCopyableInt&&) = default;

    // перемещающий оператор присваивания
    NonCopyableInt& operator=(NonCopyableInt&&) = default;
};

ostream& operator<<(ostream& os, const NonCopyableInt& val) {
    os << val.value;
    return os;
}

int main() {
    vector<int> v = {1, 2, 3};
    // vector<NonCopyableInt> vNonCopyable(v.begin(), v.end());
    // vector<NonCopyableInt> vNonCopyable(move(v.begin()), move(v.end()));

    vector<NonCopyableInt> vNonCopyable(make_move_iterator(v.begin()), make_move_iterator(v.end()));

    auto tmp = vNonCopyable[0];
    std::cout << tmp << std::endl;

    // std::cout << vNonCopyable[0] << std::endl;

    return 0;
}