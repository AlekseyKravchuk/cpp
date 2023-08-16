#include "test_runner.h"

// В деструкторе оцениваем количество упавших тестов
TestRunner::~TestRunner() {
    if (_fail_count) {
        std::cerr << "============ ATTENTION! "s << _fail_count <<" test(s) failed. Exiting! ============"s << std::endl;
        std::exit(1);
    } else {
        std::cerr << "================= ALL TESTS SUCCESSFULLY PASSED ================="s << std::endl;
    }
}

// Assert проверяет истинность аргументов только одного типа — типа bool
// Нужна нам для того, чтобы в тесте "TestAreSynonyms" не писать следующую конструкцию:
// AssertEqual(AreSynonyms(synonyms, "X", "Y"), true, "synonyms: X Y"s);
void Assert(bool value, const std::string& hint) {
    AssertEqual(value, true, hint);
}