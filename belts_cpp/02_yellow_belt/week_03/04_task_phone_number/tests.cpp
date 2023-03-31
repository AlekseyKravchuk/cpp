#include "tests.h"

#include <string>

#include "phone_number.h"

void Test_ContainsPlusSign() {
    {
        PhoneNumber phone("+7-495-111-22-33"s);
        AssertEqual(phone.GetCountryCode(), "7"s, "+7-495-111-22-33: country_code: 7"s);
    }

    {
        PhoneNumber phone("+7-495-111-22-33"s);
        AssertEqual(phone.GetCityCode(), "495"s, "+7-495-111-22-33: city_code: 495"s);
    }

    {
        PhoneNumber phone("+7-495-111-22-33"s);
        AssertEqual(phone.GetLocalNumber(), "111-22-33"s, "+7-495-111-22-33: local_number: 111-22-33"s);
    }
}

// Хочется добиться следующего поведения от unit-тестов:
// 1) Если все тесты отработали успешно, то выполняется основная часть программы.
// 2) Если хотя бы один тест упал, все тесты отрабатывают, но основная часть уже не выполняется.
void TestAll() {
    // Unit-тесты выводят результаты в СТАНДАРТНЫЙ ПОТОК ОШИБОК (std::cerr).
    // Это позволяет нам не комментировать запуск тестов при отправке в тестирующую систему.
    TestRunner tr;
    tr.RunTest(Test_ContainsPlusSign, "Test_ContainsPlusSign"s);
}  // по этой закрывающей скобке вызывается деструктор "TestRunner::~TestRunner" - для этого мы и используем функцию TestAll