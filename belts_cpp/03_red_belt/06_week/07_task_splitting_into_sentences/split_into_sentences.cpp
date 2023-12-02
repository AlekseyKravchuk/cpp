#include <algorithm>
#include <iterator>  // std::make_move_iterator
#include <ostream>
#include <vector>

#include "test_runner.h"

// Объявляем "Sentence<Token>" для произвольного типа "Token" синонимом "std::vector<Token>".
// Благодаря этому в качестве возвращаемого значения функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template <typename Token>
using Sentence = std::vector<Token>;

// Класс Token имеет метод "bool IsEndSentencePunctuation() const"
template <typename Token>
std::vector<Sentence<Token>> SplitIntoSentences(std::vector<Token> tokens) {
    // Напишите реализацию функции, не копируя объекты типа Token
    std::vector<Sentence<Token>> sentences;

    auto is_punctuation = [](Token& token) {
        return token.IsEndSentencePunctuation();
    };

    auto is_normal = [](Token& token) {
        return !token.IsEndSentencePunctuation();
    };

    bool isFirst = true;
    for (auto it = tokens.begin(); it != tokens.end();) {
        if (isFirst) {
            if (it->IsEndSentencePunctuation()) {
                auto it_normal_starts = std::find_if(it, tokens.end(), is_normal);
                sentences.push_back({std::make_move_iterator(it), make_move_iterator(it_normal_starts)});
                it = it_normal_starts;
                isFirst = false;
                continue;
            }
        }

        auto it_punct_starts = std::find_if(it, tokens.end(), is_punctuation);
        if (it_punct_starts == tokens.end()) {
            // знаков пунктуации больше нет, возможно дошли до последнего нормального токена
            sentences.push_back({std::make_move_iterator(it), make_move_iterator(it_punct_starts)});
            break;
        }

        auto it_normal_starts = std::find_if(it_punct_starts, tokens.end(), is_normal);
        sentences.push_back({std::make_move_iterator(it), make_move_iterator(it_normal_starts)});
        it = it_normal_starts;
    }

    return sentences;
}

struct TestToken {
    std::string data;
    bool is_end_sentence_punctuation = false;

    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }

    bool operator==(const TestToken& other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

std::ostream& operator<<(std::ostream& stream, const TestToken& token) {
    return stream << token.data;
}

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences необходимо написать отдельный тест.
void TestSplitting() {
    ASSERT_EQUAL(
        SplitIntoSentences(std::vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!"}})),
        std::vector<Sentence<TestToken>>({{{"Split"}, {"into"}, {"sentences"}, {"!"}}}));

    ASSERT_EQUAL(
        SplitIntoSentences(std::vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}})),
        std::vector<Sentence<TestToken>>({{{"Split"}, {"into"}, {"sentences"}, {"!", true}}}));

    ASSERT_EQUAL(
        SplitIntoSentences(std::vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true}})),
        std::vector<Sentence<TestToken>>({
            {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
            {{"Without"}, {"copies"}, {".", true}},
        }));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSplitting);
    return 0;
}
