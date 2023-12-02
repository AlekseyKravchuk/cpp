#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <class T>
ostream& operator<<(ostream& os, const vector<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class T>
ostream& operator<<(ostream& os, const set<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class K, class V>
ostream& operator<<(ostream& os, const map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& kv : m) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

template <class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
    if (t != u) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " hint: " << hint;
        }
        throw runtime_error(os.str());
    }
}

void Assert(bool b, const string& hint = {}) {
    AssertEqual(b, true, hint);
}

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

void Test_EmptyString_IsPalindrome() {
    Assert(IsPalindrom(""s));
}

void Test_OneCharacterString_IsPalindrome() {
    Assert(IsPalindrom("2"s));
}

void Test_AllCharactersMatter_IsPalindrome() {
    Assert(IsPalindrom("t  t"s));
    Assert(IsPalindrom("t - t"s));
    Assert(IsPalindrom("t \n-\n t"s));
    Assert(IsPalindrom("t \"\n-\n\" t"s));
    Assert(IsPalindrom("a \t a"s));
    Assert(IsPalindrom("a -\t- a"s));
    Assert(IsPalindrom("a -- a"s));
}

void Test_CommonCases_IsPalindrome() {
    Assert(IsPalindrom("madam"s));
    Assert(IsPalindrom("level"s));
    Assert(IsPalindrom("wasitacaroracatisaw"s));
    Assert(!IsPalindrom("ma"s));
    Assert(IsPalindrom("---"s));
    Assert(IsPalindrom("-?-"s));
    Assert(IsPalindrom("-?\?-"s));
}

void Test_IgnoreSpaces_IsPalindrome() {
    Assert(!IsPalindrom("lakomo mokal"s), "lakomo mokal"s);
    Assert(!IsPalindrom("bel   xleb"s), "bel   xleb"s);
    Assert(!IsPalindrom("i el klei"s), "i el klei"s);
    Assert(!IsPalindrom("ne vidno mord ni lap a palindrom on diven"s), "ne vidno mord ni lap a palindrom on diven"s);
    Assert(!IsPalindrom("(\"  a \")"s), "(\"  a \")"s);
}

void Test_WhenIgnoringFirstOrLastChar() {
    Assert(!IsPalindrom("abab"s), "abab"s);
    Assert(!IsPalindrom("xaba"s), "xaba"s);
    Assert(IsPalindrom("aba"s), "aba"s);
}

void Test_WhenMatchingEndsBeforeMid() {
    Assert(IsPalindrom("ababa"s), "ababa"s);
    Assert(!IsPalindrom("abxyba"s), "abxyba"s);
}

void Test_WhenMatchingIgnoringCase() {
    Assert(!IsPalindrom("Lykyl"s), "Lykyl"s);
    Assert(!IsPalindrom("Aba"s), "Aba"s);
}

void TestAll() {
    // Unit-тесты выводят результаты в СТАНДАРТНЫЙ ПОТОК ОШИБОК (std::cerr).
    // Это позволяет нам не комментировать запуск тестов при отправке в тестирующую систему.
    TestRunner tr;
    tr.RunTest(Test_EmptyString_IsPalindrome, "Test_EmptyString_IsPalindrome"s);
    tr.RunTest(Test_OneCharacterString_IsPalindrome, "Test_OneCharacterString_IsPalindrome"s);
    tr.RunTest(Test_AllCharactersMatter_IsPalindrome, "Test_AllCharactersMatter_IsPalindrome"s);
    tr.RunTest(Test_IgnoreSpaces_IsPalindrome, "Test_IgnoreSpaces_IsPalindrome"s);
    tr.RunTest(Test_WhenIgnoringFirstOrLastChar, "Test_WhenIgnoringFirstOrLastChar"s);
    tr.RunTest(Test_WhenMatchingEndsBeforeMid, "Test_WhenMatchingEndsBeforeMid"s);
    tr.RunTest(Test_WhenMatchingIgnoringCase, "Test_WhenMatchingIgnoringCase"s);
}  // по этой закрывающей скобке вызывается деструктор "TestRunner::~TestRunner" - для этого мы и используем функцию TestAll

int main() {
    TestAll();

    return 0;
}
