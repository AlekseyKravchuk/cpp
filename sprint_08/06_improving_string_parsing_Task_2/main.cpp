#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <string_view>

using namespace std;

class Translator {
   public:
    Translator() = default;

    void Add(string_view source, string_view target);

    string_view TranslateForward(string_view source) const;

    string_view TranslateBackward(string_view target) const;

   private:
    set<string> _stringsHolder;  // хранилище всех строк
    map<string_view, string_view> _forwardTranlation;
    map<string_view, string_view> _backwardTranlation;
};

void Translator::Add(string_view source, string_view target) {
    std::string sourceStr = std::string(source);
    std::string targetStr = std::string(target);

    _stringsHolder.insert(sourceStr);
    _stringsHolder.insert(targetStr);

    _forwardTranlation[*_stringsHolder.find(sourceStr)] = *_stringsHolder.find(targetStr);
    _backwardTranlation[*_stringsHolder.find(targetStr)] = *_stringsHolder.find(sourceStr);
}

string_view Translator::TranslateForward(string_view source) const {
    if (_forwardTranlation.count(source)) {
        return _forwardTranlation.at(source);
    } else {
        return std::string_view();
    }
}

string_view Translator::TranslateBackward(string_view target) const {
    if (_backwardTranlation.count(target)) {
        return _backwardTranlation.at(target);
    } else {
        return std::string_view();
    }
}

void TestSimple() {
    Translator translator;
    translator.Add(string("okno"s), string("window"s));
    translator.Add(string("stol"s), string("table"s));

    assert(translator.TranslateForward("okno"s) == "window"s);
    assert(translator.TranslateBackward("table"s) == "stol"s);
    assert(translator.TranslateForward("table"s) == ""s);
}

int main() {
    TestSimple();

    return 0;
}
