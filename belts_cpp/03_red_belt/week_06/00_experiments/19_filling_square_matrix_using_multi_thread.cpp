#include <future> // std::async
#include <iostream>
#include <iterator>
#include <vector>

#include "../../log_duration.h"
#include "../../print.h"

template <typename Iterator>
class IteratorRange {
  public:
    IteratorRange(Iterator begin, Iterator end)
        : _first(begin),
          _last(end),
          _size(distance(_first, _last)) {
    }

    Iterator begin() const {
        return _first;
    }

    Iterator end() const {
        return _last;
    }

    size_t size() const {
        return _size;
    }

  private:
    Iterator _first;
    Iterator _last;
    size_t _size;
};

template <typename Iterator>
class Paginator {
  private:
    std::vector<IteratorRange<Iterator>> _pages;

  public:
    Paginator(Iterator begin, Iterator end, size_t page_size) {
        for (size_t left = std::distance(begin, end); left > 0;) {
            size_t current_page_size = std::min(page_size, left);
            Iterator current_page_end = std::next(begin, current_page_size);
            _pages.push_back({begin, current_page_end});

            left -= current_page_size;
            begin = current_page_end;
        }
    }

    auto begin() const {
        return _pages.begin();
    }

    auto end() const {
        return _pages.end();
    }

    size_t size() const {
        return _pages.size();
    }
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
    return Paginator(std::begin(c), std::end(c), page_size);
}

template <typename ContainerOfVectors>
void GenerateMatrix_SingleThread(ContainerOfVectors& result,
                                 size_t first_row_index,
                                 size_t num_of_columns) {
    for (auto& row : result) {
        row.reserve(num_of_columns);

        for (size_t column_id = 0; column_id < num_of_columns; ++column_id) {
            row.push_back(first_row_index ^ column_id);
        }
    }
}

std::vector<std::vector<int>> GenerateMatrix_SingleThread(size_t size) {
    std::vector<std::vector<int>> result(size);
    size_t first_row_id = 0;
    GenerateMatrix_SingleThread(result, first_row_id, size);

    return result;
}

std::vector<std::vector<int>> GenerateMatrix_MultiThread(size_t size, size_t page_size) {
    std::vector<std::vector<int>> result(size);
    std::vector<std::future<void>> futures;

    size_t first_row_id = 0;

    // нарезаем вектор "result" на страницы с размером "page_size"
    for (auto page : Paginate(result, page_size)) {
        futures.push_back(std::async([page, first_row_id, size]() {
            GenerateMatrix_SingleThread(page, first_row_id, size);
        }));

        first_row_id += page_size;
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
    // 171475500000
}

int main() {
    LOG_DURATION("Total");
    const size_t matrix_size = 7000;
    // const size_t matrix_size = 5;

    // КВАДРАТНАЯ матрица будет представлять собой вектор векторов
    std::vector<std::vector<int>> matrix;
    matrix = GenerateMatrix_SingleThread(matrix_size);

    // for (const auto& row : matrix) {
    //     std::cout << row << std::endl;
    // }

    {
        LOG_DURATION("Single thread generation");
        matrix = GenerateMatrix_SingleThread(matrix_size);
    }

    // {
    //     LOG_DURATION("MULTI thread generation");
    //     matrix = GenerateMatrix_MultiThread(matrix_size, 1500);
    // }

    {
        LOG_DURATION("Single thread sum");
        std::cout << SumSingleThread(matrix) << std::endl;
    }

    return 0;
}
