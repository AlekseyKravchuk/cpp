#include "tests.h"

#include <string>

#include "test_runner.h"
#include "translator.h"

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
