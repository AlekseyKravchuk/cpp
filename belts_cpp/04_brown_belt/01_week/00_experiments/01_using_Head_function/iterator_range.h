#pragma once

#include <algorithm>

template <typename Iterator>
class IteratorRange {
  public:
    IteratorRange(Iterator first, Iterator last)
        : first(first),
          last(last) {}

    Iterator begin() const {
        return first;
    }

    Iterator end() const {
        return last;
    }

    size_t size() const {
        return last - first;
    }

  private:
    Iterator first;
    Iterator last;
};

template <typename Collection>
auto Head(Collection& c, size_t top) {
    return IteratorRange{c.begin(),
                         std::next(c.begin(), std::min(top, c.size()))};
}
