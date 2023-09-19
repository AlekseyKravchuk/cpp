#include <algorithm>
#include <future>   // std::async
#include <iostream> // for temporary debugging
#include <iterator> // std::distance, std::next, std::advance
#include <map>
#include <numeric>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#include "profile.h"
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

std::vector<std::string_view> SplitIntoWords(std::string_view s_view) {
    std::vector<std::string_view> result;

    while (true) {
        size_t space_pos = s_view.find(' ');
        result.push_back(s_view.substr(0, space_pos));

        if (space_pos == s_view.npos) {
            break;
        } else {
            while (s_view[space_pos] == ' ') {
                ++space_pos;
            }

            // откусываем от "string_view" уже обработанный кусок: в качестве параметра указываем длину префикса, который нужно откусить
            s_view.remove_prefix(space_pos);
        }
    }

    return result;
}

struct Stats {
    std::map<std::string, int> word_frequences;

    void operator+=(const Stats& other);
};

void Stats::operator+=(const Stats& other) {
    for (const auto& [word, frequency] : other.word_frequences) {
        word_frequences[word] += frequency;
    }
}

Stats ExploreRangeOfLines(const std::set<std::string>& key_words,
                          const IteratorRange<std::vector<std::string>::iterator> page) {
    Stats result;
    std::unordered_set<std::string_view> checker{key_words.begin(), key_words.end()};
    for (auto line : page) {
        for (auto s_view : SplitIntoWords(line)) {
            if (checker.count(s_view)) {
                ++result.word_frequences[std::string(s_view)];
            }
        }
    }

    return result;
}

Stats ExploreKeyWords(const std::set<std::string>& key_words, std::istream& input) {
    // Реализуйте эту функцию
    Stats result{};
    std::vector<std::string> lines;
    for (std::string line; std::getline(input, line);) {
        lines.push_back(line);
    }

    size_t num_threads = 8;
    size_t page_size = std::max(lines.size() / num_threads, 1ul);

    std::cout << "========= Inside ExploreKeyWords =========" << std::endl;

    std::vector<std::future<Stats>> futures;
    for (auto page : Paginate(lines, page_size)) {
        futures.push_back(std::async(ExploreRangeOfLines, std::ref(key_words), page));
    }

    for (auto& fu : futures) {
        result += fu.get();
    }

    return result;
}

void TestBasic() {
    const std::set<std::string> key_words = {"yangle", "rocks", "sucks", "all"};

    std::stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    const auto stats = ExploreKeyWords(key_words, ss);
    const std::map<std::string, int> expected = {
        {"yangle", 6},
        {"rocks", 2},
        {"sucks", 1}};

    ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestBasic);

    return 0;
}
