#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

using namespace std;

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file,
                     const string& func, unsigned line, const string& hint) {
    if (t != u) {
        cout << boolalpha;
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cout << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

void AssertImpl(bool expr, const string& exprAsStr,
                const string& fileName, const string& functionName,
                const int lineNumber, const string& hint) {
    if (!expr) {
        cout << fileName << "("s << lineNumber << "): "s
             << functionName << ": "s
             << "ASSERT("s << exprAsStr << ") failed."s;

        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

class Synonyms {
   public:
    void Add(const string& word1, const string& word2) {
        _dict[word1].insert(word2);
        _dict[word2].insert(word1);
    }

    size_t GetSynonymCount(const string& word) const {
        return _dict.count(word) ? _dict.at(word).size() : 0;
    }

    bool AreSynonyms(const string& word1, const string& word2) const {
        return _dict.count(word1) ? _dict.at(word1).count(word2) : 0;
    }

   private:
    map<string, set<string> > _dict;
};

void TestAddingSynonymsIncreasesTheirCount() {
    Synonyms synonyms;
    // assert(synonyms.GetSynonymCount("music"s) == 0);
    ASSERT(synonyms.GetSynonymCount("music"s) == 0);

    synonyms.Add("music"s, "melody"s);
    // Два слова являются синонимами друг друга
    ASSERT(synonyms.GetSynonymCount("music"s) == 1);

    synonyms.Add("music"s, "tune"s);
    ASSERT(synonyms.GetSynonymCount("music"s) == 2);

    // Отношение синонимичности в нашей постановке задачи не является транзитивным.
    // Поэтому слова "tune" и "melody" синонимами друг друга не являются,
    // несмотря на то что оба они являются синонимами слова "music".
    ASSERT(synonyms.GetSynonymCount("tune"s) == 1);
    ASSERT(synonyms.GetSynonymCount("melody"s) == 1);

    // cout << "Test \"TestAddingSynonymsIncreasesTheirCount\" successfully passed." << endl;
}

void TestAreSynonyms() {
    Synonyms synonyms;

    synonyms.Add("music"s, "melody"s);
    assert(synonyms.AreSynonyms("melody"s, "music"s));

    synonyms.Add("азбука"s, "алфавит"s);
    synonyms.Add("азбука"s, "букварь"s);
    assert(synonyms.AreSynonyms("азбука"s, "букварь"s));

    synonyms.Add("music"s, "melody"s);
    synonyms.Add("music"s, "tune"s);
    assert(!synonyms.AreSynonyms("melody"s, "tune"s));

    // cout << "Test \"TestAreSynonyms\" successfully passed." << endl;
}

void TestSynonyms() {
    TestAddingSynonymsIncreasesTheirCount();
    TestAreSynonyms();
}

int main() {
    TestSynonyms();

    // TEMPORARILY DISABLED
    /*     Synonyms synonyms;
    string line;

    // В случае ошибок чтения из cin произойдёт выход из цикла
    while (getline(cin, line)) {
        istringstream iss(line);
        string action;
        iss >> action;

        if (action == "ADD"s) {
            string word1, word2;
            iss >> word1 >> word2;
            synonyms.Add(word1, word2);

        } else if (action == "COUNT"s) {
            string word;
            iss >> word;

            // TODO: вывести количество синонимов word
            cout << synonyms.GetSynonymCount(word) << endl;

        } else if (action == "CHECK"s) {
            string word1, word2;
            iss >> word1 >> word2;

            // TODO: проверить, являются ли word1 и word2 синонимами
            // и вывести YES либо NO
            if (synonyms.AreSynonyms(word1, word2)) {
                cout << "YES"s << endl;
            } else {
                cout << "NO"s << endl;
            }

        } else if (action == "EXIT"s) {
            break;
        }
    } */
}