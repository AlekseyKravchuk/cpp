#include <future>
#include <iterator>
#include <map>
#include <set>
#include <sstream>  // std::stringstream
#include <string>
#include <vector>

#include "profile.h"
#include "test_runner.h"

struct Stats {
    std::map<std::string, int> word_frequences;

    void operator+=(const Stats& other);
};

void Stats::operator+=(const Stats& other) {
    for (const auto& [word, frequency] : other.word_frequences) {
        this->word_frequences[word] += frequency;
    }
}

Stats ExploreLine(const std::set<std::string>& key_words, const std::string& line) {
    Stats res;

    std::istringstream iss(line);
    std::vector<std::string> words{std::istream_iterator<std::string>{iss},
                                   std::istream_iterator<std::string>{}};

    for (const auto& word : words) {
        if (key_words.find(word) != key_words.end()) {
            ++res.word_frequences[word];
        }
    }

    return res;
}

Stats ExploreKeyWordsSingleThread(const std::set<std::string>& key_words, std::istream& input) {
    Stats res;

    for (std::string line; std::getline(input, line);) {
        res += ExploreLine(key_words, line);
    }

    return res;
}

Stats ExploreKeyWordsSingleThread_(const std::set<std::string>& key_words,
                                   const std::vector<std::string>& lines) {
    Stats res;

    for (const auto& line : lines) {
        res += ExploreLine(key_words, line);
    }

    return res;
}

Stats ExploreLines(const std::set<std::string>& key_words,
                   std::vector<std::string>::iterator begin,
                   std::vector<std::string>::iterator end) {
    Stats res;

    while (begin != end) {
        res += ExploreLine(key_words, *begin++);
    }

    return res;
}

Stats ExploreKeyWords(const std::set<std::string>& key_words, std::istream& input) {
    Stats res;

    std::vector<std::string> lines;
    for (std::string line; std::getline(input, line);) {
        lines.push_back(line);
    }

    int part_size = int(lines.size() / std::thread::hardware_concurrency());

    if (part_size < 100) {
        res = ExploreKeyWordsSingleThread_(key_words, lines);
        return res;
    }

    std::vector<std::future<Stats>> vfs;
    for (int i = 0; i < (int)lines.size(); i += part_size) {
        vfs.push_back(std::async(ExploreLines, cref(key_words),
                      lines.begin() + i, lines.begin() + std::min(i + part_size,
                      (int)lines.size())));
    }

    for (auto& fs : vfs) {
        res += fs.get();
    }

    return res;
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
}
