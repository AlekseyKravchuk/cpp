#include <iostream>
#include <utility>  // std::move

using namespace std;

struct NonmovingInt {
    int value;

    // копировать, нельзя перемещать!
    NonmovingInt(const NonmovingInt&) = default;
    NonmovingInt& operator=(const NonmovingInt&) = default;

    NonmovingInt(NonmovingInt&&) = delete;
    NonmovingInt& operator=(NonmovingInt&&) = delete;
};

struct NoncopyableInt {
    // пусть членом класса станет неперемещаемый тип
    NonmovingInt value;

    // конструктор перемещения, специальное слово "default" говорит, что компилятор должен сам решить, как будет происходить перемещение.
    // Компилятор просто применит перемещение к каждому члену класса.
    // Если для члена класса определён конструктор перемещения, компилятор будет стараться его найти.
    // Если хотя бы один из членов класса переместить нельзя, компилятор не справится с поставленной задачей и сообщит об ошибке.
    NoncopyableInt(NoncopyableInt&&) = default;

    // перемещающий оператор присваивания
    NoncopyableInt& operator=(NoncopyableInt&&) = default;


    // копировать нельзя, перемещать!
    // Вот что случится, если удалить перемещающий конструктор и оператор присваивания:
    // NoncopyableInt(const NoncopyableInt&) = delete;
    // NoncopyableInt& operator=(const NoncopyableInt&) = delete;

    NoncopyableInt(NoncopyableInt&&) = default;
    NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

int main() {
    NoncopyableInt source{1};
    NoncopyableInt target(move(source));
    cout << "Неужели перенеслось?"s << endl;
    return 0;
}