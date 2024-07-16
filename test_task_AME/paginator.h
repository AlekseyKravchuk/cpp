#pragma once

#include <iterator> // std::distance, std::next, std::advance
#include <numeric>
#include <vector>

template <typename Iterator>
class IteratorRange {
  public:
    IteratorRange(Iterator first, Iterator last)
        : _it_range(first, last) {}

    Iterator begin() const {
        return _it_range.first;
    }

    Iterator end() const {
        return _it_range.second;
    }

    Iterator begin() {
        return _it_range.first;
    }

    Iterator end() {
        return _it_range.second;
    }

    size_t size() const {
        return std::distance(_it_range.first, _it_range.second);
    }

  private:
    std::pair<Iterator, Iterator> _it_range;
};

template <typename Iterator>
class Paginator {
  public:
    Paginator(Iterator it_begin, Iterator it_end, size_t page_size) {
        int len = static_cast<int>(std::distance(it_begin, it_end));
        int n_full_pages = len / static_cast<int>(page_size);
        int remainder = len % static_cast<int>(page_size);

        Iterator page_start = it_begin;
        for (int i = 0; i < n_full_pages; ++i) {
            Iterator page_end = std::next(page_start, page_size);
            _pages.emplace_back(page_start, page_end);
            page_start = page_end;
        }

        if (remainder > 0) {
            _pages.emplace_back(page_start, it_end);
        }
    }

    auto begin() const { return _pages.begin(); }
    auto end() const { return _pages.end(); }

    auto begin() { return _pages.begin(); }
    auto end() { return _pages.end(); }

    size_t size() const {
        return _pages.size();
    }

    IteratorRange<Iterator>& operator[](size_t index) {
        return _pages[index];
    }

  private:
    std::vector<IteratorRange<Iterator>> _pages{};
};

template <typename Container>
auto Paginate(Container& c, size_t page_size) {
    return Paginator(std::begin(c), std::end(c), page_size);
}

template <typename Iterator>
auto Paginate(Iterator first, Iterator second, size_t page_size) {
    return Paginator(first, second, page_size);
}

