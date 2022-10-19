#include "log_duration.h"

using Clock = std::chrono::steady_clock;
using namespace std::literals;
using namespace std;

std::vector<int> ReverseVector(const std::vector<int>& sourceVector) {
    std::vector<int> res;
    for (int i : sourceVector) {
        res.insert(res.begin(), i);
    }

    return res;
}

std::vector<int> ReverseVector2(const std::vector<int>& sourceVector) {
    std::vector<int> res;
    for (auto it = sourceVector.rbegin(); it != sourceVector.rend(); ++it) {
        res.push_back(*it);
    }

    return res;
}

std::vector<int> ReverseVector3(const std::vector<int>& sourceVector) {
    return {sourceVector.rbegin(), sourceVector.rend()};
}


// TO DO!!!
/* Анализируем и компилируем ваше решение...
Запускаем тесты...
Задача прошла 7 из 8 проверок
Не успех(



Тест № 8 НЕ прошел проверку
причина: Решение упало
reverse_vector4 fail: Assertion failed: 0 != 1
1 unit tests failed. Terminate
подсказка: Неверная реализация функции ReverseVector4 */
std::vector<int> ReverseVector4(const std::vector<int>& sourceVector) {
    std::vector<int> reversedBits(sourceVector.size());

    int lastSourceIndex = sourceVector.size() - 1;
    for (size_t i = 0; i < lastSourceIndex; ++i) {
        reversedBits[lastSourceIndex - i] = sourceVector[i];
    }

    return reversedBits;
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

void AppendRandom(std::vector<int>& v) {
    for (int i = 0; i < static_cast<int>(v.size()); ++i) {
        v[i] = rand() % 2;
    }
}

void Operate() {
    {
        int N;
        std::cin >> N;
        // static const int N = 1 << 18;

        std::vector<int> randomBits(N);
        std::vector<int> reverseBits;

        // LOG_DURATION("Total"s);

        // "1 << 17" is equivalent to "2^17" = 131072
        // static const int N = 1 << 17;
        // static const int N = 1 << 20;

        // {
        //     LOG_DURATION("Append random v2"s);
        //     // заполним вектор случайными числами 0 и 1
        //     AppendRandom2(random_bits, N);
        // }

        // заполним вектор случайными числами 0 и 1
        AppendRandom(randomBits);

        if (N <= 100000) {
            {
                LOG_DURATION("Naive"s);
                reverseBits = ReverseVector(randomBits);
            }

            {
                LOG_DURATION("Good"s);
                reverseBits = ReverseVector2(randomBits);
            }
        } else {
            {
                LOG_DURATION("Good"s);
                reverseBits = ReverseVector2(randomBits);
            }

            {
                LOG_DURATION("Best"s);
                reverseBits = ReverseVector3(randomBits);
            }

            {
                LOG_DURATION("Your"s);
                reverseBits = ReverseVector4(randomBits);
            }
        }
    }
}

int main() {
    Operate();
}