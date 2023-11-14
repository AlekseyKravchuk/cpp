#include <cassert>

#include "log_duration.h"
#include "print.h"
#include "split_into_words.h"

// #define _GLIBCXX_DEBUG 1  // включить режим отладки

std::string GenerateText() {
    const int SIZE = 1'000'000'000;
    std::string text(SIZE, 'a');

    for (int i = 100; i < SIZE; i += 100) {
        text[i] = ' ';
    }

    return text;
}

int main() {
#ifdef _GLIBCXX_DEBUG
    // std::string text = "                               ";
    // std::string text = "       word                    ";
    // std::string text = "       word1          word2                  ";
    std::string text = "       the c++ language      has gone through a major    transformation in    the past decade                    ";

    std::vector<std::string> expected = {"the", "c++", "language", "has", "gone", "through",
                                         "a", "major", "transformation", "in", "the", "past", "decade"};

    std::vector<std::string> words = SplitIntoWords(text);
    std::vector<std::string> words_via_iterator = SplitIntoWords_via_istream_iterator(text);
    
    std::vector<std::string_view> word_views_1 = SplitIntoWordsView_v1(text);
    std::vector<std::string_view> word_views_2 = SplitIntoWordsView_v2(text);
    std::vector<std::string_view> word_views_3 = SplitIntoWordsView_v3(text);

    std::cout << "words = " << words << std::endl;
    assert(words == expected);
    assert(words_via_iterator == expected);
    assert(words == std::vector<std::string>(word_views_1.begin(), word_views_1.end()));
    assert(words == std::vector<std::string>(word_views_2.begin(), word_views_2.end()));
    assert(words == std::vector<std::string>(word_views_3.begin(), word_views_3.end()));
#else
    const std::string text = GenerateText();

    {
        LOG_DURATION("SplitIntoWords");
        const auto words = SplitIntoWords(text);
    }

    {
        LOG_DURATION("SplitIntoWords_via_istream_iterator");
        const auto words = SplitIntoWords_via_istream_iterator(text);
    }

    {
        LOG_DURATION("SplitIntoWordsView_v1");
        const auto word_views = SplitIntoWordsView_v1(text);
    }

    {
        LOG_DURATION("SplitIntoWordsView_v2");
        const auto word_views = SplitIntoWordsView_v2(text);
    }

    {
        LOG_DURATION("SplitIntoWordsView_v3");
        const auto word_views = SplitIntoWordsView_v3(text);
    }

    /*
        ВРЕМЕННЫЕ ЗАТРАТЫ для SIZE = 1'000'000'000:
            SplitIntoWords:                         9912 milliseconds
            SplitIntoWords_via_istream_iterator:    3628 milliseconds
            SplitIntoWordsView_v1:                  1648 milliseconds
            SplitIntoWordsView_v2:                  1365 milliseconds
            SplitIntoWordsView_v3:                  1306 milliseconds
    */

#endif  //_GLIBCXX_DEBUG

    return 0;
}
