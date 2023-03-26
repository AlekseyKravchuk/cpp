// #include <cassert>  // вместо стандартного assert'а используется Assert и AssertEqual
#include <fstream>  // std::ifstream
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>

using namespace std::literals;
using Synonyms = std::map<std::string, std::set<std::string>>;

#define _GLIBCXX_DEBUG 1  // включить режим отладки

/*
================= Введение в unit-тесты на примере задачи "Синонимы" =================
Два слова называются синонимами друг друга, если они имеют похожие значения. Реализуйте следующие операции над словарём синонимов:
    - ADD word1 word2 — добавить в словарь пару синонимов (word1, word2).
    - COUNT word — узнать количество синонимов слова word.
    - CHECK word1 word2 — проверить, являются ли слова word1 и word2 синонимами. Слова word1 и word2считаются синонимами, если среди запросов ADD был хотя бы один запрос ADD word1word2 или ADD word2word1.

Замечание:

Для упрощения, будем считать, что синонимы не обладают транзитивностью, то есть, если A - синоним B, а B - синоним C,
то из этого НЕ следует, что A - синоним C.
Формат ввода:
    - Сначала вводится количество запросов Q, затем Q строк с описаниями запросов.
    - Гарантируется, что в каждом запросе CHECK и ADD слова word1 и word2 различны.
    - Все слова состоят лишь из латинских букв, цифр и символов подчёркивания.

Формат вывода:
Для каждого запроса в соответствующей строке выведите ответ на него:
    - В ответ на запрос COUNT word выведите единственное целое число — количество синонимов слова word.
    - В ответ на запрос CHECK word1word2 выведите строку YES, если word1 и word2 являются синонимами, и NO в противном случае.

Ввод #1:
8
ADD program code
COUNT cipher
ADD code cipher
COUNT code
COUNT program
CHECK code program
CHECK program cipher
CHECK cpp java


Вывод #1:
0
2
1
YES
NO
NO

Ввод #2:
5
ADD program code
ADD code cipher
COUNT cipher
CHECK code program
CHECK program cipher

Вывод #2:
1
YES
NO
*/

// Выполняем декомпозицию нашего решения посредством отделения логики работы со словарем синонимов от операций ввода-вывода.
// Переносим операции обработки словаря синонимов в отдельные функции (блоки).
// Это делается для того, чтобы впоследствии каждый из этих блоков покрыть unit-тестами
void AddSynonyms(Synonyms& synonyms,
                 const std::string& first_word, const std::string& second_word) {
    // synonyms[first_word].insert(second_word);
    // synonyms[second_word].insert(first_word);

    // намеренно добавляем баг, чтобы увидеть, как "стреляют" наши Assert'ы
    synonyms[first_word].insert(second_word);
    synonyms[second_word].insert(second_word);
}

size_t GetSynonymsCount(const Synonyms& synonyms,
                        const std::string& word) {
    return (synonyms.count(word)) ? synonyms.at(word).size() : 0;

    // преднамеренно вносим ошибку в код для тестирование AssertEqual
    // return (synonyms.count(word)) ? (synonyms.at(word).size() + 1) : 0;
}

bool AreSynonyms(const Synonyms& synonyms,
                 const std::string& first_word, const std::string& second_word) {
    // // корректный код
    // if (synonyms.count(first_word)) {
    //     if (synonyms.at(first_word).count(second_word)) {
    //         return true;
    //     } else {
    //         return false;
    //     }
    // } else {
    //     return false;
    // }

    // преднамеренно вносим ошибку в код для тестирование AssertEqual
    if (synonyms.count(first_word)) {
        if (synonyms.at(first_word).count(second_word)) {
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}

template <typename T, typename U>
void AssertEqual(const T& t, const U& u, const std::string& hint) {
    if (t != u) {
        std::ostringstream oss;
        oss << "Assertion failed: "s << t << " != " << u
            << " Hint: "s << hint;
        throw std::runtime_error(oss.str());
    }
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s) {
    os << "{";
    bool isFirst = true;
    for (const auto& key : s) {
        if (!isFirst) {
            os << ", "s;
        }
        isFirst = false;
        os << key;
    }
    return os << "}";
}

template <typename KeyType, typename ValueType>
std::ostream& operator<<(std::ostream& os, const std::map<KeyType, ValueType>& m) {
    os << "{";
    bool isFirst = true;
    for (const auto& [key, value] : m) {
        if (!isFirst) {
            os << ", "s;
        }
        isFirst = false;
        os << key << ": "s << value;
    }
    return os << "}";
}

void Assert(bool b, const std::string& hint) {
    AssertEqual(b, true, hint);
}

void TestAddSynonyms() {
    {
        Synonyms empty;
        AddSynonyms(empty, "a", "b");
        const Synonyms expected = {
            {"a", {"b"}},
            {"b", {"a"}}};
        // assert(empty == expected);
        AssertEqual(empty, expected, "empty == expected"s);
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
            {"c", {"b", "a"}}};
        // assert(expected == synonyms);
        AssertEqual(expected, synonyms, "expected == synonyms"s);
    }

    std::cout << "TestAddSynonyms PASSED"s << std::endl;
}

void TestGetSynonymsCount() {
    {
        Synonyms empty;

        // заменяем вызов стандартного "assert" на наш AssertEqual
        // assert(GetSynonymsCount(empty, "a") == 0);

        AssertEqual(GetSynonymsCount(empty, "a"), 0u, "count for empty"s);
    }

    {
        Synonyms synonyms = {
            {"a", {"b", "c"}},
            {"b", {"a"}},
            {"c", {"a"}}};
        // assert(GetSynonymsCount(synonyms, "a") == 2);
        // assert(GetSynonymsCount(synonyms, "b") == 1);
        // assert(GetSynonymsCount(synonyms, "z") == 0);

        AssertEqual(GetSynonymsCount(synonyms, "a"), 2u, "count for a"s);
        AssertEqual(GetSynonymsCount(synonyms, "b"), 1u, "count for b"s);
        AssertEqual(GetSynonymsCount(synonyms, "z"), 0u, "count for z"s);
    }

    std::cout << "TestAddSynonyms PASSED"s << std::endl;
}

void TestAreSynonyms() {
    {
        Synonyms empty;
        // assert(!AreSynonyms(empty, "a", "b"));
        // assert(!AreSynonyms(empty, "b", "a"));

        Assert(!AreSynonyms(empty, "a", "b"), "empty a b"s);
        Assert(!AreSynonyms(empty, "b", "a"), "empty b a"s);
    }

    {
        Synonyms synonyms = {
            {"a", {"b", "c"}},
            {"b", {"a"}},
            {"c", {"a"}}};
        // assert(AreSynonyms(synonyms, "a", "b"));
        // assert(AreSynonyms(synonyms, "b", "a"));
        // assert(AreSynonyms(>synonyms, "a", "c"));
        // assert(AreSynonyms(synonyms, "c", "a"));
        // assert(!AreSynonyms(synonyms, "b", "c"));
        // assert(!AreSynonyms(synonyms, "c", "b"));

        Assert(AreSynonyms(synonyms, "a", "b"), "synonyms: a b"s);
        Assert(AreSynonyms(synonyms, "b", "a"), "synonyms: b a"s);
        Assert(AreSynonyms(synonyms, "a", "c"), "synonyms: a c"s);
        Assert(AreSynonyms(synonyms, "c", "a"), "synonyms: c a"s);
        Assert(!AreSynonyms(synonyms, "b", "c"), "synonyms: b c"s);
        Assert(!AreSynonyms(synonyms, "c", "b"), "synonyms: c b"s);
    }
    std::cout << "TestAreSynonyms PASSED"s << std::endl;
}

template <typename TestFunc>
void RunTest(TestFunc func, const std::string& test_name) {
    try {
        func();
    } catch (const std::exception& e) {
        std::cout << test_name << " fail: "s << e.what() << std::endl;
    }
}

int main() {
#ifdef _GLIBCXX_DEBUG
    // === debug mode is enabled ===
    std::string path = "20_unit_tests_synonyms_test.txt"s;
    std::ifstream in(path);

    if (!in) {
        throw std::runtime_error("File \""s + path + "\" wasn't opened");
    }

    // сохраняем указатель на "streambuf"
    std::streambuf* cinbuf_bkp = std::cin.rdbuf();
    // перенаправляем поток ввода std::cin на файл "input.txt"
    std::cin.rdbuf(in.rdbuf());  // redirect std::cin to "input.txt"!
#endif                           // END OF _GLIBCXX_DEBUG

    enum class OPCODE {
        ADD,
        COUNT,
        CHECK
    };

    std::map<std::string, OPCODE> str2opcode{
        {"ADD"s, OPCODE::ADD},
        {"COUNT"s, OPCODE::COUNT},
        {"CHECK"s, OPCODE::CHECK}};

    int num_queries;

    // считываем количество запросов, которое нам дано
    std::cin >> num_queries;

    // объявляем словарь синонимов (отображение из строки в множество строк)
    std::map<std::string, std::set<std::string>> synonyms;

    for (int i = 0; i < num_queries; ++i) {
        std::string operation_code;
        std::cin >> operation_code;

        switch (str2opcode[operation_code]) {
            case OPCODE::ADD: {
                std::string first_word, second_word;
                std::cin >> first_word >> second_word;
                AddSynonyms(synonyms, first_word, second_word);
                break;
            }
            case OPCODE::COUNT: {
                std::string word;
                std::cin >> word;
                std::cout << GetSynonymsCount(synonyms, word) << std::endl;
                break;
            }
            case OPCODE::CHECK: {
                std::string first_word, second_word;
                std::cin >> first_word >> second_word;
                std::cout << ((AreSynonyms(synonyms, first_word, second_word)) ? "YES"s : "NO"s) << std::endl;
                break;
            }
            default: {
                throw std::runtime_error("Invalid operation code: " + operation_code);
            }
        }
    }

#ifdef _GLIBCXX_DEBUG
    std::cin.rdbuf(cinbuf_bkp);  // восстанавливаем standard input

    // Запускаем unit-тесты
    std::cout << "======================= TESTS ======================="s << std::endl;
    RunTest(TestAddSynonyms, "TestAddSynonyms"s);
    RunTest(TestAreSynonyms, "TestAreSynonyms"s);
    RunTest(TestGetSynonymsCount, "TestGetSynonymsCount"s);
    std::cout << "===================== END TESTS ====================="s << std::endl;
#endif

    return 0;
}
