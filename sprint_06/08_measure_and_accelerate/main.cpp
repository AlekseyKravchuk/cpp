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

std::vector<int> ReverseVector4(const std::vector<int>& sourceVector) {
    std::vector<int> reversedBits(sourceVector.size());

    int i = static_cast<int>(sourceVector.size()) - 1;
    for (auto it = sourceVector.begin(); it != sourceVector.end(); ++it) {
        reversedBits[i--] = *it;
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

        std::vector<int> randomBits(N);
        std::vector<int> reverseBits;

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