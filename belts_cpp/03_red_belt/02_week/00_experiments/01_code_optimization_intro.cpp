#include <chrono>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;
using namespace std::chrono;

// вариант со вставкой элементов в начало вектора (и с последующим сдвигом всех элементов массива)
std::vector<std::string> GenerateBigVector() {
    // запоминаем момент, когда наша функция начала работать
    auto start = std::chrono::steady_clock::now();

    std::vector<std::string> result;
    int vec_size = 30'000;
    for (int i = 0; i < vec_size; ++i) {
        result.insert(std::begin(result), std::to_string(i));
    }

    // запоминаем момент окончания работы функции
    auto finish = std::chrono::steady_clock::now();

    auto duration = finish - start;
    std::cerr
        << "GenerateBigVector completed in "s
        << std::chrono::duration_cast<milliseconds>(duration).count()
        << " milliseconds"s << std::endl;
    return result;
}

// вариант без резервирования
std::vector<std::string> GenerateBigVectorEnhanced_V1() {
    // запоминаем момент, когда наша функция начала работать
    auto start = std::chrono::steady_clock::now();

    std::vector<std::string> result;
    int vec_size = 30'000;
    for (int i = 0; i < vec_size; ++i) {
        result.push_back(std::to_string(i));
    }

    // запоминаем момент окончания работы функции
    auto finish = std::chrono::steady_clock::now();

    auto duration = finish - start;
    std::cerr
        << "GenerateBigVectorEnhanced_V1 (without std::vector::reserve) completed in "s
        << std::chrono::duration_cast<milliseconds>(duration).count()
        << " milliseconds"s << std::endl;
    return result;
}

// вариант с резервированием
std::vector<std::string> GenerateBigVectorEnhanced_V2() {
    // запоминаем момент, когда наша функция начала работать
    auto start = std::chrono::steady_clock::now();

    std::vector<std::string> result;
    int vec_size = 30'000;
    result.reserve(vec_size);
    for (int i = 0; i < vec_size; ++i) {
        result.push_back(std::to_string(i));
    }

    // запоминаем момент окончания работы функции
    auto finish = std::chrono::steady_clock::now();

    auto duration = finish - start;
    std::cerr
        << "GenerateBigVectorEnhanced_V2 (USING std::vector::reserve) completed in "s
        << std::chrono::duration_cast<milliseconds>(duration).count()
        << " milliseconds"s << std::endl;

    return result;
}

int main() {
    // запоминаем момент, когда наша программа начала работать
    auto start = std::chrono::steady_clock::now();

    // std::cout << "Size of generated vector: "s << GenerateBigVector().size() << std::endl;
    // std::cout << "Size of generated vector: "s << GenerateBigVectorEnhanced_V2().size() << std::endl;
    std::vector<std::string> v = GenerateBigVector();
    v = GenerateBigVectorEnhanced_V1();
    v = GenerateBigVectorEnhanced_V2();

    // запоминаем момент окончания работы программы
    auto finish = std::chrono::steady_clock::now();

    auto duration = finish - start;
    std::cerr
        << "Program completed in "s
        << std::chrono::duration_cast<milliseconds>(duration).count()
        << " milliseconds"s << std::endl;

    return 0;
}
