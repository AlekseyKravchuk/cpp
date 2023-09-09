#include <algorithm>
#include <future>
#include <future> // собираемся запускать асинхронные операции
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

#include "../../log_duration.h"
#include "paginator.h"

int SumToVectors(const std::vector<int>& one,
                 const std::vector<int>& two) {

    std::future<int> f = std::async([&one, &two]() {
        return std::accumulate(std::begin(one), std::end(one), 0);
    });

    int result = std::accumulate(std::begin(two), std::end(two), 0);

    return result + f.get();
}

template <typename ContainerOfVectors>
void GenerateSingleThread(ContainerOfVectors& result,
                          size_t first_row,
                          size_t column_size) {
    for (auto& row : result) {
        row.reserve(column_size);

        for (size_t column = 0; column < column_size; ++column) {
            row.push_back(first_row ^ column); // '^' - побитовая операция XORS
        }

        ++first_row;
    }
}

std::vector<std::vector<int>> GenerateSingleThread(size_t size) {
    std::vector<std::vector<int>> result(size);
    GenerateSingleThread(result, 0, size);

    return result;
}

std::vector<std::vector<int>> GenerateMultiThread(size_t size, size_t page_size) {
    std::vector<std::vector<int>> result(size);

    // хотим разбить вектор "result" на несколько страниц (частей)
    std::vector<std::future<void>> futures;
    size_t first_row = 0;

    for (auto page : Paginate(result, page_size)) {
        futures.push_back(std::async([page, first_row, size]() {
            // "first_row" - номер первой строки, "size" размер колонки (длина строки)
            GenerateSingleThread(page, first_row, size);
        }));

        first_row += page_size;
    }

    return result;
}

template <typename ContainerOfVectors>
int64_t SumSingleThread(const ContainerOfVectors& matrix) {
    int64_t sum = 0;

    for (const auto& row : matrix) {
        for (auto item : row) {
            sum += item;
        }
    }

    return sum;
}

int main() {
    // std::cout << SumToVectors({1, 1, 1, 1}, {3, 3, 3}) << std::endl;

    LOG_DURATION("Total");
    const size_t matrix_size = 7000;
    std::vector<std::vector<int>> matrix;

    {
        LOG_DURATION("Single thread generation");
        matrix = GenerateSingleThread(matrix_size);
    }

    {
        LOG_DURATION("Multi thread generation");
        matrix = GenerateMultiThread(matrix_size, 2000); // 2000 - размер страницы
    }

    {
        {
            LOG_DURATION("Single thread sum");
            std::cout << SumSingleThread(matrix) << std::endl;
        }
    }

    return 0;
}
