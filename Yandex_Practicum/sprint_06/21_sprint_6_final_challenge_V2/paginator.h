#pragma once

#include <cmath>     // std::abs
#include <fstream>   // std::ostream
#include <iterator>  // std::distance, std::next
#include <utility>   // std::pair
#include <vector>    // std::vector

template <typename Iterator>
class IteratorRange {
   public:
    IteratorRange(Iterator pageBeginIt, Iterator pageEndIt) : _p(pageBeginIt, pageEndIt) {}

    Iterator begin() const {
        return _p.first;
    }

    Iterator end() const {
        return _p.second;
    }

    size_t size() const {
        return std::abs(std::distance(_p.first, _p.second));
    }

   private:
    std::pair<Iterator, Iterator> _p;
};

template <typename Iterator>
class Paginator {
   public:
    // constructor
    Paginator(Iterator beginIt, Iterator endIt, int pageSize) {
        auto pageStartIt = beginIt;
        auto pageEndIt = beginIt;
        int wholeNumOfPages = std::distance(beginIt, endIt) / pageSize;
        int remainder = std::distance(beginIt, endIt) % pageSize;

        if (wholeNumOfPages > 0) {
            // initialization of the end of page
            pageEndIt = std::next(pageStartIt, pageSize);

            for (int i = 0; i < wholeNumOfPages; ++i) {
                _pages.push_back(IteratorRange<Iterator>(pageStartIt, pageEndIt));
                pageStartIt = pageEndIt;
                pageEndIt = std::next(pageStartIt, pageSize);
            }
        }

        if (remainder > 0) {
            _pages.push_back(IteratorRange<Iterator>(pageStartIt, endIt));
        }
    }

    size_t size() const {
        return _pages.size();
    }

    auto begin() const {
        return _pages.begin();
    }

    auto end() const {
        return _pages.end();
    }

   private:
    std::vector<IteratorRange<Iterator>> _pages;
};

template <typename Container>
auto Paginate(const Container& container, size_t pageSize) {
    return Paginator(begin(container), end(container), pageSize);
}


template <typename Iterator>
std::ostream& operator<<(std::ostream& os, const IteratorRange<Iterator>& range) {
    for (auto it = range.begin(); it != range.end(); ++it) {
        os << *it;
    }
    return os;
}

