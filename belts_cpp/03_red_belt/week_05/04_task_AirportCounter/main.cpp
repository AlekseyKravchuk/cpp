#include "log_duration.h"
#include "test_runner.h"
#include "tests.h"

int main() {
    TestRunner tr;

    // По условию, суммарное время работы всех тестов не должно превышать двух секунд.
    // Если ваше время будет лишь чуть больше двух секунд, попробуйте отправить ваше решение в тестирующую систему.
    // Возможно, там более мощное железо, и ваше решение будет принято.
    // Кроме того, не забудьте включить оптимизации при компиляции кода.

    LOG_DURATION("Total tests duration");
    RUN_TEST(tr, TestMoscow);
    RUN_TEST(tr, TestManyConstructions);
    RUN_TEST(tr, TestManyGetItems);
    RUN_TEST(tr, TestMostPopularAirport);

    return 0;
}