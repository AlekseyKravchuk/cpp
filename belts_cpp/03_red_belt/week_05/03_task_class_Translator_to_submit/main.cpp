#include <map>
#include <set>
#include <string_view>
#include <vector>

#include "test_runner.h"

class Translator {
   public:
    void Add(std::string_view source, std::string_view target);
    std::string_view TranslateForward(std::string_view source) const;
    std::string_view TranslateBackward(std::string_view target) const;

   private:
    std::set<std::string> _sources;
    std::set<std::string> _targets;
    std::map<std::string_view, std::vector<std::string_view>> _forward;
    std::map<std::string_view, std::vector<std::string_view>> _backward;
};

void Translator::Add(std::string_view source, std::string_view target) {
    // std::string_view actual_source = *_sources.insert(std::string(source)).first;
    // std::string_view actual_target = *_targets.insert(std::string(target)).first;

    std::string_view actual_source = *_sources.emplace(source).first;
    std::string_view actual_target = *_targets.emplace(target).first;

    _forward[actual_source].push_back(actual_target);
    _backward[actual_target].push_back(actual_source);
}

std::string_view Translator::TranslateForward(std::string_view source) const {
    if (!_sources.count(std::string(source))) {
        return {};
    } else {
        return _forward.at(source).back();
    }
}

std::string_view Translator::TranslateBackward(std::string_view target) const {
    if (!_targets.count(std::string(target))) {
        return {};
    } else {
        return _backward.at(target).back();
    }
}

// ========================= TESTS =========================
void TestSimple() {
    Translator translator;
    translator.Add(std::string("okno"), std::string("window"));
    translator.Add(std::string("stol"), std::string("table"));

    ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
    ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
    ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

void TestAdvanced() {
    {
        Translator translator;
        translator.Add(std::string("okno"), std::string("window"));
        translator.Add(std::string("okno"), std::string("proem"));
        translator.Add(std::string("okno"), std::string("illuminator"));

        ASSERT_EQUAL(translator.TranslateForward("okno"), "illuminator");
    }

    {
        Translator translator;
        translator.Add(std::string("okno"), std::string("window"));
        translator.Add(std::string("okno"), std::string("proem"));
        translator.Add(std::string("okno"), std::string("illuminator"));

        ASSERT_EQUAL(translator.TranslateForward("okno"), "illuminator");
    }
}
// ====================== END OF TESTS =====================

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSimple);
    RUN_TEST(tr, TestAdvanced);

    return 0;
}