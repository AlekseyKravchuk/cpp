#pragma once

#include <iterator> // std::distance, std::next, std::advance
#include <numeric>
#include <vector>

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
        size_t len = std::distance(it_container_begin, it_container_end);
        size_t n_full_pages = len / page_size;
        size_t remainder = len % page_size;

        Iterator page_start = it_container_begin;
        for (size_t i = 0; i < n_full_pages; ++i) {
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
