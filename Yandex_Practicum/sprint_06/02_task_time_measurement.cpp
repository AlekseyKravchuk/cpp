#include <chrono>
#include <cstdlib>  // std::rand
#include <iostream>
#include <vector>

using namespace std::literals;

std::vector<int> ReverseVector(const std::vector<int>& sourceVector) {
    std::vector<int> res;
    for (int i : sourceVector) {
        res.insert(res.begin(), i);
    }

    return res;
}

int CountPops(const std::vector<int>& sourceVector, int begin, int end) {
    int res = 0;

    for (int i = begin; i < end; ++i) {
        if (sourceVector[i]) {
            ++res;
        }
    }

    return res;
}

void AppendRandom(std::vector<int>& v, int n) {
    for (int i = 0; i < n; ++i) {
        v.push_back(std::rand() % 2);
    }
}

void Operate() {
    std::vector<int> random_bits;

    // one bit shift left << is equivalent to multiplying number by 2
    static const int N = 1 << 17;

    const std::chrono::steady_clock::time_point startTimeAppendRandom = std::chrono::steady_clock::now();
    AppendRandom(random_bits, N);  // заполним вектор случайными числами 0 и 1
    const std::chrono::steady_clock::time_point endTimeAppendRandom = std::chrono::steady_clock::now();
    const std::chrono::steady_clock::duration durAppendRandom = endTimeAppendRandom - startTimeAppendRandom;

    const std::chrono::steady_clock::time_point startTimeReverseVector = std::chrono::steady_clock::now();
    std::vector<int> reversedBits = ReverseVector(random_bits);  // revert given vector
    const std::chrono::steady_clock::time_point endTimeReverseVector = std::chrono::steady_clock::now();
    const std::chrono::steady_clock::duration durReverseVector = endTimeReverseVector - startTimeReverseVector;

    const std::chrono::steady_clock::time_point startTimeCounting = std::chrono::steady_clock::now();
    // посчитаем процент единиц на начальных отрезках вектора
    for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
        // чтобы вычислить проценты, мы умножаем на литерал 100. типа double;
        // целочисленное значение функции CountPops при этом автоматически
        // преобразуется к double, как и i
        double rate = CountPops(reversedBits, 0, i) * 100. / i;
        std::cout << "After "s << i << " bits we found "s << rate << "% pops"s
                  << std::endl;
    }
    const std::chrono::steady_clock::time_point endTimeCounting = std::chrono::steady_clock::now();
    const std::chrono::steady_clock::duration durCounting = endTimeCounting - startTimeCounting;

    std::cerr << "Append random: "s << std::chrono::duration_cast<std::chrono::milliseconds>(durAppendRandom).count() << " ms"s << std::endl;
    std::cerr << "Reverse: "s << std::chrono::duration_cast<std::chrono::milliseconds>(durReverseVector).count() << " ms"s << std::endl;
    std::cerr << "Counting: "s << std::chrono::duration_cast<std::chrono::milliseconds>(durCounting).count() << " ms"s << std::endl;
}

int main() {
    Operate();
    return 0;
}