#include <map>
#include <sstream>  // std::stringstream
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#include "log_duration.h"
// #include "profile.h"  // uncomment to submit solution
#include "test_runner.h"

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

    void operator+=(const Stats& other) {
        for (const auto& [word, frequence] : other.word_frequences) {
            this->word_frequences[word] += frequence;
        }
    }
};

// // Lines below is commented. They are intended for MULTI THREAD solution
// Stats ExploreLine(const std::set<std::string>& key_words, const std::string& line) {
// }

// Stats ExploreKeyWordsSingleThread(
//     const std::set<std::string>& key_words, std::istream& input) {
//     Stats result;
//     for (std::string line; std::getline(input, line);) {
//         result += ExploreLine(key_words, line);
//     }
//     return result;
// }

Stats ExploreKeyWords(const std::set<std::string>& key_words, std::istream& input) {
    // Реализуйте эту функцию
    Stats result;
    std::string line;
    std::unordered_set<std::string_view> checker{key_words.begin(), key_words.end()};

    while (std::getline(input, line)) {
        for (auto s_view : SplitIntoWords(line)) {
            if (checker.count(s_view)) {  // надеемся на то, что поиск текущего слова в checker'e займет O(1), а не O(n)
                ++result.word_frequences[std::string(s_view)];
            }
        }
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
}
