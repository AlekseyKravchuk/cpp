#include <algorithm>
#include <future>
#include <future>  // собираемся запускать асинхронные операции
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

#include "paginator.h"
#include "profile.h"

using namespace std;

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

// генерируется квадратная матрица размера "NxN", ОДНОпоточная версия
template <typename ContainerOfVectors>
void GenerateSingleThread(ContainerOfVectors& result,
                          size_t first_row_id,
                          size_t N) {
    for (auto& row : result) {
        row.reserve(N);

        for (size_t column = 0; column < N; ++column) {
            row.push_back(first_row_id ^ column);  // '^' - побитовая операция XOR
        }

        ++first_row_id;
    }
}

vector<vector<int>> GenerateSingleThread(size_t size) {
    vector<vector<int>> result(size);
    size_t first_row_id = 0;
    GenerateSingleThread(result, first_row_id, size);

    return result;
}

vector<vector<int>> GenerateMultiThread(size_t size, size_t page_size) {
    vector<vector<int>> result(size);

    // хотим разбить вектор "result" на несколько страниц (частей)
    vector<future<void>> futures;
    size_t first_row_id = 0;

    for (auto page : Paginate(result, page_size)) {
        // складываем в вектор "futures" результаты вызова async
        futures.push_back(std::async([page, first_row_id, size]() {
            GenerateSingleThread(page, first_row_id, size);
        }));

        first_row_id += page_size;
    }

    return result;
}

int main() {
    LOG_DURATION("Total");
    const size_t matrix_size = 7000;
    const size_t page_size = 500;
    std::vector<std::vector<int>> matrix;

    {
        LOG_DURATION("Single thread generation");
        matrix = GenerateSingleThread(matrix_size);
    }

    {
        LOG_DURATION("Multi thread generation");
        matrix = GenerateMultiThread(matrix_size, page_size); // 2000 - размер страницы
    }

    {
        {
            LOG_DURATION("Single thread sum");
            std::cout << SumSingleThread(matrix) << std::endl;
            // сумма должна получиться 195928050144
        }

        {
            LOG_DURATION("MULTI thread sum");
            std::cout << SumSingleThread(matrix) << std::endl;
        }
    }

    return 0;
}
