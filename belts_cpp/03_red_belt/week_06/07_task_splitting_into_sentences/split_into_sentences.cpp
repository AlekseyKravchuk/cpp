#include <ostream>
#include <vector>

#include "test_runner.h"

// Объявляем "Sentence<Token>" для произвольного типа "Token" синонимом "std::vector<Token>".
// Благодаря этому в качестве возвращаемого значения функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template <typename Token>
using Sentence = std::vector<Token>;

// TODO:
/*
Failed case #1/22: (Wrong answer)

Input:
pairs
4
Split 0
into 0
sentences 0
! 0

Your output:

Correct output:
Split_0 into_0 sentences_0 !_0
 (Time used: 0.00/1.00, preprocess time used: 0/None, memory used: 65413120/536870912
*/
// Класс Token имеет метод "bool IsEndSentencePunctuation() const"
template <typename Token>
std::vector<Sentence<Token>> SplitIntoSentences(std::vector<Token> tokens) {
    // Напишите реализацию функции, не копируя объекты типа Token
    std::vector<Sentence<Token>> sentences;
    Sentence<Token> sentence;

    for (auto& token : tokens) {
        if (!token.IsEndSentencePunctuation()) {
            sentence.push_back(std::move(token));
        } else {
            if (!sentence.empty()) {
                sentences.push_back(std::move(sentence));
            }
        }
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
