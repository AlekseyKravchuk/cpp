#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

int main() {
    vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
    vector<string> words = {"some"s, "words"s, "to"s, "test"s};
    auto foundIt = find(langs.begin(), langs.end(), "Java"s);

    // ========== варианты использования метода std::string::insert ==========
    // /* 1 */ langs.insert(foundIt, "Go"s);
    // /* 2 */ langs.insert(foundIt, words.begin(), words.end());

    /* 3 */
    // int timesToRepeat = 5;
    // const string value = "TEST";
    // langs.insert(foundIt, timesToRepeat, value);

    /* 4 */ langs.insert(foundIt, {"1"s, "2"s, "3"s});

    PrintRange(langs.begin(), langs.end());
}