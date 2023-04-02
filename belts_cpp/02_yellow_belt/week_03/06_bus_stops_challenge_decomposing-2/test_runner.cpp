#include "test_runner.h"

// В деструкторе оцениваем количество упавших тестов
TestRunner::~TestRunner() {
    if (_fail_count) {
        std::cerr << "============ ATTENTION! Some TESTS failed. Exiting! ============"s << std::endl;
        std::exit(1);
    } else {
        std::cerr << "================= ALL TESTS SUCCESSFULLY PASSED ================="s << std::endl;
    }
}