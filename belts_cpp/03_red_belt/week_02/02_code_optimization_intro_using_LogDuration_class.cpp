#include <vector>

#include "00_log_duration.h"

// вариант со вставкой элементов в начало вектора (и с последующим сдвигом всех элементов массива)
std::vector<std::string> GenerateBigVector() {
    std::vector<std::string> result;
    {
        // LogDuration log_duration("GenerateBigVector"s);
        LOG_DURATION("GenerateBigVector"s);
        int vec_size = 30'000;
        for (int i = 0; i < vec_size; ++i) {
            result.insert(std::begin(result), std::to_string(i));
        }
    }
    return result;
}

// вариант без резервирования
std::vector<std::string> GenerateBigVectorEnhanced_V1() {
    std::vector<std::string> result;
    {
        // LogDuration log_duration("GenerateBigVectorEnhanced_V1"s);
        LOG_DURATION("GenerateBigVectorEnhanced_V1"s);
        int vec_size = 30'000;
        for (int i = 0; i < vec_size; ++i) {
            result.push_back(std::to_string(i));
        }
    }
    return result;
}

// вариант с резервированием
std::vector<std::string> GenerateBigVectorEnhanced_V2() {
    std::vector<std::string> result;
    {
        // LogDuration log_duration("GenerateBigVectorEnhanced_V2"s);
        LOG_DURATION("GenerateBigVectorEnhanced_V2"s);
        int vec_size = 30'000;
        result.reserve(vec_size);
        for (int i = 0; i < vec_size; ++i) {
            result.push_back(std::to_string(i));
        }
    }
    return result;
}

int main() {
    {
        // LogDuration log_duration("Total"s);
        LOG_DURATION("Total"s);
        std::vector<std::string> v = GenerateBigVector();
        v = GenerateBigVectorEnhanced_V1();
        v = GenerateBigVectorEnhanced_V2();
    }

    return 0;
}
