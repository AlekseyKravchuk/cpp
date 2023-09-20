#include <algorithm>
#include <execution>
#include <future>
#include <future> // собираемся запускать асинхронные операции
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

#include "../../log_duration.h"
#include "paginator.h"

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

// генерируется квадратная матрица размера "NxN", ОДНОпоточная версия.
// "N" - количество строк и количество столбцов.
template <typename ContainerOfVectors>
void GenerateSingleThread(ContainerOfVectors& result,
                          size_t first_row_id,
                          size_t N) {
    // // СТАРЫЙ вариант реализации функции
    // for (auto& row : result) {
    //     row.reserve(N);

    //     for (size_t column = 0; column < N; ++column) {
    //         row.push_back(first_row_id ^ column); // '^' - побитовая операция XORS
    //     }

    //     ++first_row_id;
    // }

    std::for_each(
        std::execution::par,
        std::begin(result),
        std::end(result),
        [&first_row_id, N](std::vector<int>& row) {
            row.reserve(N);
            for (size_t column = 0; column < N; ++column) {
                row.push_back(static_cast<int>(first_row_id ^ column));
            }
            ++first_row_id;
        });
}

std::vector<std::vector<int>> GenerateSingleThread(size_t size) {
    std::vector<std::vector<int>> result(size);
    size_t first_row_id = 0;
    GenerateSingleThread(result, first_row_id, size);

    return result;
}

int main() {
    LOG_DURATION("Total");
    const size_t matrix_size = 7000;
    std::vector<std::vector<int>> matrix;

    {
        LOG_DURATION("Single thread generation");
        matrix = GenerateSingleThread(matrix_size);
    }

    {
        LOG_DURATION("Single thread sum");
        std::cout << SumSingleThread(matrix) << std::endl;
        // сумма должна получиться 195928050144
    }

    return 0;
}
