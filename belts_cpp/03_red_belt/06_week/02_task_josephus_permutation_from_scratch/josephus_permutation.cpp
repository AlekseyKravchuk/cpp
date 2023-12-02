#include <cstdint>
#include <iterator>
#include <list>
#include <numeric>
#include <vector>

#include "test_runner.h"

template <typename Container, typename ForwardIT>
ForwardIT LoopIterator(Container& container, ForwardIT it) {
    return it == std::end(container) ? std::begin(container) : it;
}

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
    std::list<typename RandomIt::value_type> pool(std::make_move_iterator(first), std::make_move_iterator(last));
    auto it_pos = std::begin(pool);

    while (!pool.empty()) {
        *(first++) = std::move(*it_pos);

        if (pool.size() == 1u) {
            break;
        }

        it_pos = LoopIterator(pool, pool.erase(it_pos));

        for (uint32_t i = 1; i < step_size; ++i) {
            it_pos = LoopIterator(pool, std::next(it_pos));
        }
    }
}
// // ==================================================================================================================

// // the same but via lambda function
// template <typename RandomIt>
// void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
//     std::list<typename RandomIt::value_type> pool(std::make_move_iterator(first), std::make_move_iterator(last));
//     auto it_pos = std::begin(pool);

//     auto LoopIterator = [&pool](decltype(std::begin(pool)) it) {
//         return it == std::end(pool) ? std::begin(pool) : it;
//     };

//     while (!pool.empty()) {
//         *(first++) = std::move(*it_pos);

//         if (pool.size() == 1u) {
//             break;
//         }

//         it_pos = LoopIterator(pool.erase(it_pos));

//         for (uint32_t i = 1; i < step_size; ++i) {
//             it_pos = LoopIterator( std::next(it_pos));
//         }
//     }
// }
// // ==================================================================================================================

std::vector<int> MakeTestVector() {
    std::vector<int> numbers(10);
    std::iota(std::begin(numbers), std::end(numbers), 0);
    return numbers;
}

void TestIntVector() {
    const std::vector<int> numbers = MakeTestVector();
    {
        std::vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(std::begin(numbers_copy), std::end(numbers_copy), 1);
        ASSERT_EQUAL(numbers_copy, numbers);
    }
    {
        std::vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(std::begin(numbers_copy), std::end(numbers_copy), 3);
        ASSERT_EQUAL(numbers_copy, std::vector<int>({0, 3, 6, 9, 4, 8, 5, 2, 7, 1}));
    }
}

// Это специальный тип, который поможет вам убедиться, что ваша реализация
// функции MakeJosephusPermutation не выполняет копирование объектов.
// Сейчас вы, возможно, не понимаете как он устроен, однако мы расскажем,
// почему он устроен именно так, далее в блоке про move-семантику —
// в видео «Некопируемые типы»

struct NoncopyableInt {
    int value;

    NoncopyableInt(const NoncopyableInt&) = delete;
    NoncopyableInt& operator=(const NoncopyableInt&) = delete;

    NoncopyableInt(NoncopyableInt&&) = default;
    NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator==(const NoncopyableInt& lhs, const NoncopyableInt& rhs) {
    return lhs.value == rhs.value;
}

std::ostream& operator<<(std::ostream& os, const NoncopyableInt& v) {
    return os << v.value;
}

void TestAvoidsCopying() {
    std::vector<NoncopyableInt> numbers;
    numbers.push_back({1});
    numbers.push_back({2});
    numbers.push_back({3});
    numbers.push_back({4});
    numbers.push_back({5});

    MakeJosephusPermutation(begin(numbers), end(numbers), 2);

    std::vector<NoncopyableInt> expected;
    expected.push_back({1});
    expected.push_back({3});
    expected.push_back({5});
    expected.push_back({4});
    expected.push_back({2});

    ASSERT_EQUAL(numbers, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestIntVector);
    RUN_TEST(tr, TestAvoidsCopying);
    return 0;
}
