#include "utilities.h"

#include <random>

using namespace std;

vector<int> GenerateRandomVectorOfInt(int sample_size, int seed) {
    // Инициализируем генератор случайных чисел с заданным начальным фиксированным значением (seed)
    std::mt19937 gen(static_cast<unsigned int>(seed));
    std::uniform_int_distribution<int> distr(5, 1'000);

    std::vector<int> random_numbers;
    random_numbers.reserve(static_cast<size_t>(sample_size));

    for (int i = 0; i < sample_size; ++i) {
        random_numbers.push_back(distr(gen));
    }

    return random_numbers;
}


void CountingSort_SingleThread(vector<int>::iterator first,
                               vector<int>::iterator second,
                               vector<unsigned int>::iterator it_counter_begin,
                               int min_val,
                               int max_val) {
    if (first == second) {
        return;
    }

    // заполняем counter
    for (auto it = first; it != second; ++it) {
        *(it_counter_begin + (*it - min_val)) += 1;  // эквивалент "++counter[*it - min_val];"
    }

    // восстанавливаем кусок вектора (в отсортированном виде), на который указывают итераторы first и second
    size_t n_repetitions;
    for (auto i = min_val; i <= max_val; ++i) {
        n_repetitions = *(it_counter_begin + (i - min_val));  // эквивалент "n_repetitions = counter[i - min_val];"

        for (auto j = 0U; j < n_repetitions; ++j) {
            *(first++) = i;
        }
    }
}