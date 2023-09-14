#include <future>
#include <iterator>
#include <numeric>  // std::accumulate
#include <vector>

#include "test_runner.h"

template <typename Iterator>
class IteratorRange {
   public:
    IteratorRange(Iterator first, Iterator last)
        : _p(first, last) {}

    Iterator begin() const {
        return _p.first;
    }

    Iterator end() const {
        return _p.second;
    }

    // ==================================
    Iterator begin() {
        return _p.first;
    }

    Iterator end() {
        return _p.second;
    }
    // ==================================

    size_t size() const {
        return std::distance(_p.first, _p.second);
    }

   private:
    std::pair<Iterator, Iterator> _p;
};

template <typename Iterator>
class Paginator {
   public:
    Paginator(Iterator it_container_begin, Iterator it_container_end, size_t page_size) {
        int len = std::distance(it_container_begin, it_container_end);
        int n_full_pages = len / static_cast<int>(page_size);
        int remainder = len % static_cast<int>(page_size);

        Iterator page_start = it_container_begin;
        for (int i = 0; i < n_full_pages; ++i) {
            Iterator page_end = std::next(page_start, page_size);
            _pages.emplace_back(page_start, page_end);
            page_start = page_end;
        }

        if (remainder > 0) {
            _pages.emplace_back(page_start, it_container_end);
        }
    }

    auto begin() const { return _pages.begin(); }
    auto end() const { return _pages.end(); }

    size_t size() const {
        return _pages.size();
    }

   private:
    std::vector<IteratorRange<Iterator>> _pages{};
};

template <typename Container>
auto Paginate(Container& c, size_t page_size) {
    return Paginator(std::begin(c), std::end(c), page_size);
}

int64_t CalculateMatrixSum(const std::vector<std::vector<int>>& matrix) {
    // хотим разбить вектор "matrix" на несколько страниц (частей)
    std::vector<std::future<int64_t>> futures;
    size_t page_size = 1'000;

    for (auto page : Paginate(matrix, page_size)) {
        int64_t acc = 0;

        // складываем в вектор "futures" результаты вызова async
        futures.push_back(std::async([page, &acc]() {
            for (const auto& v : page) {
                acc = std::accumulate(v.begin(), v.end(), acc);
            }
            return acc;
        }));
    }

    int64_t result = 0;
    for (auto& f : futures) {
        result += f.get();
    }

    return result;
}

void TestCalculateMatrixSum() {
    const std::vector<std::vector<int>> matrix = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}};

    ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
    ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestCalculateMatrixSum);
}
