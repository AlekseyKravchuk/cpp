#include <cmath>  // std::abs
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "test_runner.h"

using namespace std::literals;

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

    size_t size() const {
        return std::abs(std::distance(_it_range.first, _it_range.second));
    }

   private:
    std::pair<Iterator, Iterator> _it_range;
};

// Реализуйте шаблон класса Paginator
template <typename Iterator>
class Paginator {
   public:
    Paginator(Iterator it_container_begin, Iterator it_container_end, size_t page_size)
        : _page_size(page_size) {
    }

    Iterator begin() { return _it_begin; }
    Iterator end() { return _it_end; }

    size_t size() const {
        return _page_size;
    }

   private:
    size_t _page_size{};
    std::vector<IteratorRange<Iterator>> _pages{};
};

template <typename Container>
auto Paginate(Container& c, size_t page_size) {
    // Реализуйте этот шаблон функции
}

void TestPageCounts() {
    std::vector<int> v(15);

    ASSERT_EQUAL(Paginate(v, 1).size(), v.size());
    ASSERT_EQUAL(Paginate(v, 3).size(), 5u);
    ASSERT_EQUAL(Paginate(v, 5).size(), 3u);
    ASSERT_EQUAL(Paginate(v, 4).size(), 4u);
    ASSERT_EQUAL(Paginate(v, 15).size(), 1u);
    ASSERT_EQUAL(Paginate(v, 150).size(), 1u);
    ASSERT_EQUAL(Paginate(v, 14).size(), 2u);
}

void TestLooping() {
    vector<int> v(15);
    std::iota(begin(v), end(v), 1);

    Paginator<vector<int>::iterator> paginate_v(v.begin(), v.end(), 6);
    ostringstream os;
    for (const auto& page : paginate_v) {
        for (int x : page) {
            os << x << ' ';
        }
        os << '\n';
    }

    ASSERT_EQUAL(os.str(), "1 2 3 4 5 6 \n7 8 9 10 11 12 \n13 14 15 \n");
}

void TestModification() {
    std::vector<std::string> vs = {"one", "two", "three", "four", "five"};
    for (auto page : Paginate(vs, 2)) {
        for (auto& word : page) {
            word[0] = toupper(word[0]);
        }
    }

    const std::vector<std::string> expected = {"One", "Two", "Three", "Four", "Five"};
    ASSERT_EQUAL(vs, expected);
}

void TestPageSizes() {
    std::string letters(26, ' ');

    Paginator letters_pagination(letters.begin(), letters.end(), 11);
    std::vector<size_t> page_sizes;
    for (const auto& page : letters_pagination) {
        page_sizes.push_back(page.size());
    }

    const std::vector<size_t> expected = {11, 11, 4};
    ASSERT_EQUAL(page_sizes, expected);
}

void TestConstContainer() {
    const std::string letters = "abcdefghijklmnopqrstuvwxyz";

    std::vector<std::string> pages;
    for (const auto& page : Paginate(letters, 10)) {
        pages.push_back(string(page.begin(), page.end()));
    }

    const std::vector<std::string> expected = {"abcdefghij", "klmnopqrst", "uvwxyz"};
    ASSERT_EQUAL(pages, expected);
}

void TestPagePagination() {
    std::vector<int> v(22);
    std::iota(begin(v), end(v), 1);

    vstd::ector<std::vector<int>> lines;
    for (const auto& split_by_9 : Paginate(v, 9)) {
        for (const auto& split_by_4 : Paginate(split_by_9, 4)) {
            lines.push_back({});
            for (int item : split_by_4) {
                lines.back().push_back(item);
            }
        }
    }

    const vector<vector<int>> expected = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9},
        {10, 11, 12, 13},
        {14, 15, 16, 17},
        {18},
        {19, 20, 21, 22}};
    ASSERT_EQUAL(lines, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestPageCounts);
    RUN_TEST(tr, TestLooping);
    RUN_TEST(tr, TestModification);
    RUN_TEST(tr, TestPageSizes);
    RUN_TEST(tr, TestConstContainer);
    RUN_TEST(tr, TestPagePagination);
}
