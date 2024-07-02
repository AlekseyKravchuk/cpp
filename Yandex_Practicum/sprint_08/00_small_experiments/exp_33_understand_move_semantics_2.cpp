#include <cassert>
#include <iostream>
#include <list>
#include <numeric>
#include <ostream>
#include <vector>

using namespace std;

class A {
   public:
    int _val;

    // обычный конструктор с параметром
    A(int num) : _val(num) {}

    // сообщаем компилятору, что конструктора копирования у класса не существует.
    // A(const A&) = delete;

    // добавили конструктор копирования в явном виде
    A(const A& other) {
        _val = other._val;
        std::cout << "class A: copy constructor called." << std::endl;
    }

    // добавили оператор присваивания в явном виде
    A& operator=(const A& other) {
        _val = other._val;

        return *this;
    }

    // сообщаем компилятору, что оператора присваивания у класса не существует.
    // A& operator=(const A&) = delete;

    // конструктор перемещения
    A(A&&) = default;

    // перемещающий оператор присваивания
    A& operator=(A&&) = default;
};

ostream& operator<<(ostream& os, const A& val) {
    os << val._val;
    return os;
}

int main() {
    // vector<A> v = {1, 2, 3, 4, 5};
    // vector<A> v = {std::move(1), {2}, {3}, {4}};
    vector<A> v = std::move(A(1));
    // vector<A> v(v.begin(), v.end());
    // vector<A> v(move(v.begin()), move(v.end()));

    // vector<A> v(make_move_iterator(v.begin()), make_move_iterator(v.end()));

    // auto tmp = v[0];
    // std::cout << tmp << std::endl;
    std::cout << v[0] << std::endl;

    // std::cout << v[0] << std::endl;

    return 0;
}