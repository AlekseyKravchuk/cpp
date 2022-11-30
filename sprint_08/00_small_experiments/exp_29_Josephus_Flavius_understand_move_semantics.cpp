#include <cassert>
#include <iostream>
#include <list>
#include <numeric>
#include <vector>

using namespace std;

// ========================= РАБОЧАЯ версия функции, исключающая копирование объектов =========================
template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
    
    // Тип переупорядочиваемых объектов можно получить с помощью выражения "typename RandomIt::value_type"
    std::list<typename RandomIt::value_type> pool(make_move_iterator(first), make_move_iterator(last));

    size_t cur_pos = 0;

    while (!pool.empty()) {
        auto it = pool.begin();

        advance(it, cur_pos);
        *(first++) = move(*it);

        it = pool.erase(it);

        if (pool.empty()) {
            break;
        }

        cur_pos = (cur_pos + step_size - 1) % pool.size();
    }
}

// ========================= Версия функции, предложенная в треде Slack'а =========================
// template <typename RandomIt>
// void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
//     vector<typename RandomIt::value_type> pool;
//     std::move(first, last, back_inserter(pool));

//     size_t cur_pos = 0;

//     while (first != last) {
//         *(first++) = std::move(*(pool.begin() + cur_pos));
//         pool.erase(pool.begin() + cur_pos);
//         if (first == last) {
//             break;
//         }
//         cur_pos = (cur_pos + step_size - 1) % pool.size();
//     }
// }

vector<int> MakeTestVector() {
    vector<int> numbers(10);
    iota(begin(numbers), end(numbers), 0);
    return numbers;
}

void TestIntVector() {
    const vector<int> numbers = MakeTestVector();
    // {
    //     vector<int> numbers_copy = numbers;
    //     MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
    //     assert(numbers_copy == numbers);
    // }
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
        
        // numbers_copy.size() = 10
        assert(numbers_copy == vector<int>({0, 3, 6, 9, 4, 8, 5, 2, 7, 1}));
    }
}

// Это специальный тип, который поможет вам убедиться, что ваша реализация
// функции MakeJosephusPermutation не выполняет копирование объектов.
struct NoncopyableInt {
    int value;

    // ================== Запрещаем копировать объекты класса ================== 
    // сообщаем компилятору, что конструктора копирования у класса не существует.
    NoncopyableInt(const NoncopyableInt&) = delete;

    // сообщаем компилятору, что оператора присваивания у класса не существует.
    NoncopyableInt& operator=(const NoncopyableInt&) = delete;
    // ==========================================================================

    // конструктор перемещения, специальное слово "default" говорит, что компилятор должен сам решить, как будет происходить перемещение.
    // Компилятор просто применит перемещение к каждому члену класса.
    // Если для члена класса определён конструктор перемещения, компилятор будет стараться его найти.
    // Если хотя бы один из членов класса переместить нельзя, компилятор не справится с поставленной задачей и сообщит об ошибке.

    // Вместо "const NoncopyableInt&" при перемещении конструктор получает "NoncopyableInt&&".
    // Это тоже ссылка на объект типа "NoncopyableInt", но необычная. Не ссылка на ссылку, а специальная "rvalue-ссылка".
    NoncopyableInt(NoncopyableInt&&) = default;  // ===> конструктор ПЕРЕМЕЩЕНИЯ

    // перемещающий оператор присваивания
    NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator==(const NoncopyableInt& lhs, const NoncopyableInt& rhs) {
    return lhs.value == rhs.value;
}

ostream& operator<<(ostream& os, const NoncopyableInt& v) {
    return os << v.value;
}

void TestAvoidsCopying() {
    vector<NoncopyableInt> numbers;
    numbers.push_back({1});
    numbers.push_back({2});
    numbers.push_back({3});
    numbers.push_back({4});
    numbers.push_back({5});

    MakeJosephusPermutation(begin(numbers), end(numbers), 2);

    vector<NoncopyableInt> expected;
    expected.push_back({1});
    expected.push_back({3});
    expected.push_back({5});
    expected.push_back({4});
    expected.push_back({2});

    assert(numbers == expected);
}

int main() {
    TestIntVector();
    TestAvoidsCopying();
}