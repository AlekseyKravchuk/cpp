#include "synonyms.h"
#include "test_runner.h"
#include "tests.h"

void TestAll() {
    TestRunner test_runner;
    RUN_TEST(test_runner, TestAddSynonyms);
    RUN_TEST(test_runner, TestCount);
    RUN_TEST(test_runner, TestAreSynonyms);
}  // здесь вызывается деструктор класса "TestRunner"

int main() {
    TestAll();

    int querries_number;
    std::cin >> querries_number;  // считываем количество запросов, которое нам даны

    // словарь, который хранит для данной строки множество её синонимов
    Synonyms synonyms;

    for (int i = 0; i < querries_number; ++i) {
        std::string operation_code;
        std::cin >> operation_code;

        if (operation_code == "ADD") {
            std::string first_word, second_word;
            std::cin >> first_word >> second_word;
            AddSynonyms(synonyms, first_word, second_word);

        } else if (operation_code == "COUNT") {
            std::string word;
            std::cin >> word;
            std::cout << GetSynonymsCount(synonyms, word) << std::endl;
        } else if (operation_code == "CHECK") {
            std::string first_word, second_word;
            std::cin >> first_word >> second_word;
            AreSynonyms(synonyms, first_word, second_word);
        }
    }

    return 0;
}