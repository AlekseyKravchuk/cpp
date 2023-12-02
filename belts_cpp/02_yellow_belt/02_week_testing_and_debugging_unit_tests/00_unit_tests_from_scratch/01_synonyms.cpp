#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <string>

using Synonyms = std::map<std::string, std::set<std::string>>;

// выполняем декомпозицию задачи "Синонимы"

void AddSynonyms(Synonyms& synonyms,
                 const std::string& first_word,
                 const std::string& second_word) {
    // synonyms[first_word].insert(second_word);
    // synonyms[second_word].insert(first_word);

    synonyms[first_word].insert(second_word);
    synonyms[second_word].insert(second_word);
}

size_t GetSynonymsCount(Synonyms& synonyms,
                        const std::string& word) {
    return synonyms[word].size();
}

bool AreSynonyms(Synonyms& synonyms,
                 const std::string& first_word,
                 const std::string& second_word) {
    return synonyms[first_word].count(second_word);
}

// =============================== TESTS ===============================
void TestAddSynonyms() {
    {
        Synonyms empty;
        AddSynonyms(empty, "a", "b");

        const Synonyms expected = {
            {"a", {"b"}},
            {"b", {"a"}}};

        assert(empty == expected);
    }

    {
        Synonyms synonyms = {
            {"a", {"b"}},
            {"b", {"a", "c"}},
            {"c", {"b"}}};

        AddSynonyms(synonyms, "a", "c");
        Synonyms expected = {
            {"a", {"b", "c"}},
            {"b", {"a", "c"}},
            {"c", {"a", "b"}}};

        assert(synonyms == expected);
    }

    std::cout << "TestAddSynonyms OK" << std::endl;
}

void TestCount() {
    {
        Synonyms empty;
        assert(GetSynonymsCount(empty, "a") == 0);
    }

    {
        Synonyms synonyms = {
            {"a", {"b", "c"}},
            {"b", {"a"}},
            {"c", {"a"}}};

        assert(GetSynonymsCount(synonyms, "a") == 2);
        assert(GetSynonymsCount(synonyms, "b") == 1);
        assert(GetSynonymsCount(synonyms, "z") == 0);
    }

    std::cout << "TestCount OK" << std::endl;
}

void TestAreSynonyms() {
    {
        Synonyms emtpy;
        assert(!AreSynonyms(emtpy, "a", "b"));
        assert(!AreSynonyms(emtpy, "b", "a"));
    }

    {
        Synonyms synonyms = {
            {"a", {"b", "c"}},
            {"b", {"a"}},
            {"c", {"a"}}};

        assert(AreSynonyms(synonyms, "a", "b"));
        assert(AreSynonyms(synonyms, "b", "a"));
        assert(AreSynonyms(synonyms, "a", "c"));
        assert(AreSynonyms(synonyms, "c", "a"));
        assert(!AreSynonyms(synonyms, "b", "c"));
        assert(!AreSynonyms(synonyms, "c", "b"));
    }

    std::cout << "TestAreSynonyms OK" << std::endl;
}

void TestAll() {
    TestAddSynonyms();
    TestCount();
    TestAreSynonyms();
}

// ============================ END of TESTS ============================

int main() {
    TestAll();

    // int querries_number;
    // std::cin >> querries_number;  // считываем количество запросов, которое нам даны

    // // словарь, который хранит для данной строки множество её синонимов
    // Synonyms synonyms;

    // for (int i = 0; i < querries_number; ++i) {
    //     std::string operation_code;
    //     std::cin >> operation_code;

    //     if (operation_code == "ADD") {
    //         std::string first_word, second_word;
    //         std::cin >> first_word >> second_word;
    //         AddSynonyms(synonyms, first_word, second_word);

    //     } else if (operation_code == "COUNT") {
    //         std::string word;
    //         std::cin >> word;
    //         std::cout << GetSynonymsCount(synonyms, word) << std::endl;
    //     } else if (operation_code == "CHECK") {
    //         std::string first_word, second_word;
    //         std::cin >> first_word >> second_word;
    //         AreSynonyms(synonyms, first_word, second_word);
    //     }
    // }

    return 0;
}
