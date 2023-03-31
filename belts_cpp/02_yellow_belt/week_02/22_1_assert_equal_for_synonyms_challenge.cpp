// #include <cassert>  // вместо стандартного assert'а используется Assert и AssertEqual
#include <fstream>  // std::ifstream
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>

using namespace std::literals;
using Synonyms = std::map<std::string, std::set<std::string>>;

void AddSynonyms(Synonyms& synonyms,
                 const std::string& first_word, const std::string& second_word) {
    synonyms[first_word].insert(second_word);
    synonyms[second_word].insert(first_word);

    // // ==== намеренно добавляем баг, чтобы увидеть, как "стреляют" наши Assert'ы ====
    // synonyms[first_word].insert(second_word);
    // synonyms[second_word].insert(second_word);
}

size_t GetSynonymsCount(const Synonyms& synonyms,
                        const std::string& word) {
    return (synonyms.count(word)) ? synonyms.at(word).size() : 0;

    // преднамеренно вносим ошибку в код для тестирование AssertEqual
    // return (synonyms.count(word)) ? (synonyms.at(word).size() + 1) : 0;
}

// функция проверяет, являются ли 2 слова синонимами
bool AreSynonyms(const Synonyms& synonyms,
                 const std::string& first_word, const std::string& second_word) {
    // корректный код
    if (synonyms.count(first_word)) {
        if (synonyms.at(first_word).count(second_word)) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }

    // // ==== преднамеренно вносим ошибку в код для тестирование AssertEqual ====
    // if (synonyms.count(first_word)) {
    //     if (synonyms.at(first_word).count(second_word)) {
    //         return false;
    //     } else {
    //         return true;
    //     }
    // } else {
    //     return false;
    // }
}

// AssertEqual должна сравнивать аргументы любых типов
template <typename T, typename U>
void AssertEqual(T t, U u, const std::string hint) {
    if (t != u) {
        std::ostringstream oss;
        oss << "Assertion failed: " << t << " != " << u
            << ", Hint: " << hint;
        throw std::runtime_error(oss.str());
    }
}

// Assert проверяет истинность аргументов только одного типа — типа bool
// Нужна нам для того, чтобы в тесте "TestAreSynonyms" не писать следующую конструкцию:
// AssertEqual(AreSynonyms(synonyms, "X", "Y"), true, "synonyms: X Y"s);
void Assert(bool bool_value_to_check, const std::string& hint) {
    AssertEqual(bool_value_to_check, true, hint);
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

void TestAddSynonyms() {
    {
        Synonyms empty;
        AddSynonyms(empty, "a", "b");
        const Synonyms expected = {
            {"a", {"b"}},
            {"b", {"a"}}};
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
        AssertEqual(expected, synonyms, "expected == synonyms"s);
    }
}

void TestGetSynonymsCount() {
    {
        Synonyms empty;
        AssertEqual(GetSynonymsCount(empty, "a"), 0u, "count for empty"s);
    }

    {
        Synonyms synonyms = {
            {"a", {"b", "c"}},
            {"b", {"a"}},
            {"c", {"a"}}};

        AssertEqual(GetSynonymsCount(synonyms, "a"), 2u, "count for a"s);
        AssertEqual(GetSynonymsCount(synonyms, "b"), 1u, "count for b"s);
        AssertEqual(GetSynonymsCount(synonyms, "z"), 0u, "count for z"s);
    }
}

void TestAreSynonyms() {
    {
        Synonyms empty;
        Assert(!AreSynonyms(empty, "a", "b"), "empty a b"s);
        Assert(!AreSynonyms(empty, "b", "a"), "empty b a"s);
    }

    {
        Synonyms synonyms = {
            {"a", {"b", "c"}},
            {"b", {"a"}},
            {"c", {"a"}}};

        Assert(AreSynonyms(synonyms, "a", "b"), "synonyms: a b"s);
        Assert(AreSynonyms(synonyms, "b", "a"), "synonyms: b a"s);
        Assert(AreSynonyms(synonyms, "a", "c"), "synonyms: a c"s);
        Assert(AreSynonyms(synonyms, "c", "a"), "synonyms: c a"s);
        Assert(!AreSynonyms(synonyms, "b", "c"), "synonyms: b c"s);
        Assert(!AreSynonyms(synonyms, "c", "b"), "synonyms: c b"s);
    }
}

// Класс "TestRunner" управляет запуском unit-тестов. Создаем объект класса "TestRunner".
// Вызываем посредством этого объекта метод "RunTest" по числу имеющихся тестов.
// После того, как все тесты будут выполнены через "RunTest", нужно будет выполнить некоторые дейстия, а именно:
// посчитать количество упавших тестов и если оно больше нуля, то завершить работу программы.
// Указанный функционал реализован в деструкторе "~TestRunner".
class TestRunner {
   public:
    template <typename FuncType>
    void RunTest(FuncType test_as_func, std::string test_name) {
        try {
            if (!_times_runtest_called) {
                std::cerr << "====================== Beginning of TESTS ======================"s << std::endl;
            }
            test_as_func();
            ++_times_runtest_called;
            std::cerr << test_name << " PASSED"s << std::endl;
        } catch (const std::exception& e) {
            std::cerr << test_name << " fail: "s << e.what() << std::endl;
            ++_times_runtest_called;  // также нужно учитывать вызовы "RunTest", завершившихся падением
            ++_fail_count;
        }
    }

    // В деструкторе оцениваем количество упавших тестов
    ~TestRunner() {
        if (_fail_count) {
            std::cerr << "============ ATTENTION! Some TESTS failed. Exiting! ============"s << std::endl;
            std::exit(1);
        } else {
            std::cerr << "================= ALL TESTS SUCCESSFULLY PASSED ================="s << std::endl;
        }
    }

   private:
    // счетчик числа упавших тестов
    int _fail_count = 0;

    // счетчик количества запусков метода "RunTest"
    inline static int _times_runtest_called = 0;
};

// Хочется добиться следующего поведения от unit-тестов:
// 1) Если все тесты отработали успешно, то выполняется основная часть программы.
// 2) Если хотя бы один тест упал, все тесты отрабатывают, но основная часть уже не выполняется.
void TestAll() {
    // Unit-тесты выводят результаты в СТАНДАРТНЫЙ ПОТОК ОШИБОК (std::cerr).
    // Это позволяет нам не комментировать запуск тестов при отправке в тестирующую систему.
    TestRunner tr;
    tr.RunTest(TestAddSynonyms, "TestAddSynonyms"s);
    tr.RunTest(TestAreSynonyms, "TestAreSynonyms"s);
    tr.RunTest(TestGetSynonymsCount, "TestGetSynonymsCount"s);
} // по этой закрывающей скобке вызывается деструктор "TestRunner::~TestRunner" - для этого мы и используем функцию TestAll

class RedirectStandardInput {
   public:
    RedirectStandardInput(std::ifstream& input, std::string path) {
        input.open(path, std::ios::in);
        if (!input) {
            throw std::runtime_error("File \""s + path + "\" is not opened"s);
        }

        // сохраняем указатель на "streambuf"
        _cinbuf_bkp = std::cin.rdbuf();

        // перенаправляем поток ввода std::cin на файл
        std::cin.rdbuf(input.rdbuf());
    }

    ~RedirectStandardInput() {
        std::cin.rdbuf(_cinbuf_bkp);  // восстанавливаем standard input
    }

   private:
    std::streambuf* _cinbuf_bkp{nullptr};
};

int main() {
    // Запускаем unit-тесты
    // Функция "TestAll" должна идти самым первым вызовом в функции main
    TestAll();

    // ========= redirection of standard input to the file =========
    std::string path = "20_unit_tests_synonyms_test.txt"s;
    std::ifstream input;
    RedirectStandardInput redirection(input, path);
    // ================== END of redirection ========================

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
    std::cin >> num_queries;  // считываем количество запросов, которое нам дано

    // объявляем словарь синонимов: каждому слову соответствует МНОЖЕСТВО слов-синонимов
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

    return 0;
}  // по закрывающей скобке будет вызван деструктор класса "RedirectStandardInput", в котором будет восстановлен стандартный ввод
