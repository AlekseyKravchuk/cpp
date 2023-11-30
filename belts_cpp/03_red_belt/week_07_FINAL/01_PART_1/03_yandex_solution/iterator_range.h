#pragma once

#include <algorithm>
using namespace std;

template <typename It>
class IteratorRange {
public:
  IteratorRange(It first, It last) : first(first), last(last) {
  }

  It begin() const {
    return first;
  }

  It end() const {
    return last;
  }

  size_t size() const {
    return last - first;
  }

private:
  It first, last;
};

template <typename Container>
auto Head(Container& c, int top) {
  return IteratorRange(begin(c), begin(c) + min<size_t>(max(top, 0), c.size()));
}

template <typename Container>
auto MakeRange(Container& c) {
    return IteratorRange{begin(c), end(c)};
}

template <typename Container>
auto ReverseRange(const Container& c) {
  return IteratorRange{rbegin(c), rend(c)};
}

template <typename Iterator>
auto Advance(Iterator begin, Iterator end, int count) {
    while (begin != end && count--) { ++begin; }
    return begin;
}