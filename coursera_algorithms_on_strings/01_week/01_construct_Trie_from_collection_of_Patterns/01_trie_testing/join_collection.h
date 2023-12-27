#pragma once

#include <algorithm> // std::min
#include <iterator>  // std::begin
#include <set>
#include <sstream>
#include <string_view>
#include <vector>

template <typename It>
class IteratorRange {
  public:
    IteratorRange(It first, It last) : first(first), last(last) {}

    It begin() const { return first; }

    It end() const { return last; }

    size_t size() const { return last - first; }

  private:
    It first{}, last{};
};

template <typename Container>
auto Head(Container& c, int top) {
    return IteratorRange(std::begin(c),
                         std::begin(c) + std::min<size_t>(std::max(top, 0),
                                                          c.size()));
}

template <typename Container>
std::string Join(char c, const Container& cont) {
    std::ostringstream os;

    for (const auto& item : Head(cont, cont.size() - 1)) {
        os << item << c;
    }
    os << *rbegin(cont);
    return os.str();
}

template <typename Container>
std::string JoinAddExtraNewline(char c, const Container& cont) {
    return Join(c, cont).append("\n");
}

std::set<std::string> SplitBy(std::string_view s, char sep) {
    std::set<std::string> result;

    while (!s.empty()) {
        size_t pos = s.find(sep);
        result.insert(std::string(s.substr(0, pos)));
        s.remove_prefix(pos != s.npos ? pos + 1 : s.size());
    }
    return result;
}
