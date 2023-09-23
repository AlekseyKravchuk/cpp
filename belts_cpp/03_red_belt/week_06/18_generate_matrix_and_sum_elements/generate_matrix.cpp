#include <algorithm>
#include <cassert>
#include <chrono>
#include <execution>
#include <future> // собираемся запускать асинхронные операции
#include <iostream>
#include <iterator>
#include <mutex>
#include <numeric> // std::reduce
#include <thread>
#include <vector>

#include "../../log_duration.h"
#include "paginator.h"
#include "test_runner.h"

// // =================================== SINGLE Threads ====================================

// генерируется квадратная матрица размера "NxN", ОДНОпоточная версия.
template <typename ContainerOfVectors>
void GenerateSingleThread(ContainerOfVectors& result,
                          size_t first_row_id,
                          size_t N) {
    for (auto& row : result) {
        row.reserve(N); // для каждой строки матрицы вызываем "std::vector::reserve(N)"

        for (size_t column = 0; column < N; ++column) {
            row.push_back(first_row_id ^ column); // '^' - побитовая операция XOR
        }

        ++first_row_id;
    }
}

std::vector<std::vector<int>> GenerateSingleThread(size_t size) {
    std::vector<std::vector<int>> result(size); // создаем вектор из "size" векторов (вектор из "size" векторов нулевой длины)
    size_t first_row_id = 0;
    GenerateSingleThread(result, first_row_id, size);

    return result;
}

template <typename ContainerOfVectors>
uint64_t SumSingleThread(const ContainerOfVectors& matrix) {
    uint64_t sum = 0;

    for (const auto& row : matrix) {
        for (auto item : row) {
            sum += item;
        }
    }

    return sum;
}
// // ================================ End of SINGLE Threads ================================

// // ================================ MULTIThread via ASYNC ================================
std::vector<std::vector<int>> GenerateMultiThread(size_t size, size_t page_size) {
    std::vector<std::vector<int>> result(size); // создаем вектор из "size" векторов (вектор из "size" векторов нулевой длины)
    size_t first_row_id = 0;

    // std::future<T>, созданный с помощью std::async вызывает get() в своем деструкторе
    std::vector<std::future<void>> futures;

    for (auto page : Paginate(result, page_size)) {
        // складывая объекты типа "std::future<void>" в вектор, мы откладываем момент их разрушения (вызова деструктора)
        futures.push_back(
            std::async([page, first_row_id, size]() { // помещаем в вектор ВРЕМЕННЫЙ объект - он туда ПЕРЕМЕЩАЕТСЯ
                GenerateSingleThread(page, first_row_id, size);
            }));
        first_row_id += page_size;
    }

    return result;
} // Здесь вызываются деструкторы всех фьюч, которые мы сложили в вектор. Эти фьючи в своих деструкторах вызывают std::future<void>::get()
  // и дожидаются окончания асинхронных операций.

// // ============================ End of MULTIThread via ASYNC =============================

// // ========================== MULTIThread via Execution Policy ===========================

// генерируется квадратная матрица размера "NxN", МНОГОпоточная версия, реализованная посредством добавления
// первого аргумента "ExecutionPolicy" в стандартном алгоритме "std::for_each".
template <typename ContainerOfVectors>
void GenerateMultiThread_ExecutionPolicy(ContainerOfVectors& result,
                                         size_t first_row_id,
                                         size_t N) {
    std::for_each(
        std::execution::seq,
        std::begin(result),
        std::end(result),
        [&first_row_id, N](auto& row) {
            row.reserve(N);
            for (size_t column = 0; column < N; ++column) {
                row.push_back(first_row_id ^ column);
            }
            ++first_row_id;
        });
}

std::vector<std::vector<int>> GenerateMultiThread_ExecutionPolicy(size_t size) {
    std::vector<std::vector<int>> result(size); // создаем вектор из "size" векторов (вектор из "size" векторов нулевой длины)
    size_t first_row_id = 0;
    GenerateMultiThread_ExecutionPolicy(result, first_row_id, size);

    return result;
}

template <typename ContainerOfVectors>
uint64_t SumMultiThread_ExecutionPolicy(const ContainerOfVectors& matrix) {
    // // std::execution - МЕРТВОРОЖДЕННОЕ ТВОРЕНИЕ, НЕ ИСПОЛЬЗОВАТЬ !!!
    // int64_t sum = 0;

    // std::for_each(
    //     std::execution::par,
    //     std::begin(matrix),
    //     std::end(matrix),
    //     [&](const std::vector<int>& row) {
    //         sum += std::reduce(row.begin(), row.end());
    //     });

    // return sum;

    // int64_t ans = 0;
    // std::for_each(std::execution::par, matrix.begin(), matrix.end(),
    //               [&](const auto& arr) { ans += std::reduce(std::execution::par, arr.begin(), arr.end()); });
    uint64_t sum = 0ul;
    for (const auto& row : matrix) {
        sum += std::reduce(row.begin(), row.end());
    }
    return sum;
}

// // ========================== ENd of MULTIThread via Execution Policy ===========================

void Test_SingleThread_and_MultiThread_AreEqual() {
    const size_t matrix_size = 7000;
    auto m1 = GenerateSingleThread(matrix_size);

    size_t threads_num = 4;
    size_t page_size = std::max(matrix_size / threads_num, 1ul);
    auto m2 = GenerateMultiThread(matrix_size, page_size);

    for (size_t row_idx = 0; row_idx < matrix_size; ++row_idx) {
        ASSERT_EQUAL(m1[row_idx], m2[row_idx]);
    }
}

void Test_SingleThread_and_ExecutionPolicy_AreEqual() {
    const size_t matrix_size = 7000;
    auto m1 = GenerateSingleThread(matrix_size);

    auto m2 = GenerateMultiThread_ExecutionPolicy(matrix_size);
    for (size_t row_idx = 0; row_idx < matrix_size; ++row_idx) {
        ASSERT_EQUAL(m1[row_idx], m2[row_idx]);
    }
}

void TestAll() {
    TestRunner tr;
    RUN_TEST(tr, Test_SingleThread_and_MultiThread_AreEqual);
    RUN_TEST(tr, Test_SingleThread_and_ExecutionPolicy_AreEqual);
}

int main() {
    TestAll();

    {
        LOG_DURATION("Total");
        const size_t matrix_size = 7000;
        std::vector<std::vector<int>> matrix;

        // {
        //     LOG_DURATION("Single thread generation");
        //     matrix = GenerateSingleThread(matrix_size);
        // }

        // {
        //     LOG_DURATION("MULTI thread generation");
        //     size_t threads_num = 4;
        //     size_t page_size = std::max(matrix.size() / threads_num, 1ul);
        //     matrix = GenerateMultiThread(matrix_size, page_size);
        // }

        {
            LOG_DURATION("ExecutionPolicy generation");
            matrix = GenerateMultiThread_ExecutionPolicy(matrix_size);
        }

        // {
        //     LOG_DURATION("Single thread sum");
        //     std::cout << "SumSingleThread(matrix) = " << SumSingleThread(matrix) << std::endl;
        // }

        {
            LOG_DURATION("Multi thread sum (Execution Policy)");
            std::cout << "SumMultiThread_ExecutionPolicy(matrix) = " << SumMultiThread_ExecutionPolicy(matrix) << std::endl;
        }
    }

    return 0;
}
