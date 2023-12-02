#include "tests.h"

#include <string>

#include "phone_number.h"

void Test_1() {
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

void Test_2() {
    {
        PhoneNumber phone("+7-495-1112233"s);
        AssertEqual(phone.GetCountryCode(), "7"s, "+7-495-1112233: country_code: 7"s);
    }

    {
        PhoneNumber phone("+7-495-1112233"s);
        AssertEqual(phone.GetCityCode(), "495"s, "+7-495-1112233: city_code: 495"s);
    }

    {
        PhoneNumber phone("+7-495-1112233"s);
        AssertEqual(phone.GetLocalNumber(), "1112233"s, "+7-495-1112233: local_number: 1112233"s);
    }
}

void Test_3() {
    {
        PhoneNumber phone("+323-22-460002"s);
        AssertEqual(phone.GetCountryCode(), "323"s, "+323-22-460002: country_code: 323"s);
    }

    {
        PhoneNumber phone("+323-22-460002"s);
        AssertEqual(phone.GetCityCode(), "22"s, "+323-22-460002: city_code: 22"s);
    }

    {
        PhoneNumber phone("+323-22-460002"s);
        AssertEqual(phone.GetLocalNumber(), "460002"s, "+323-22-460002: local_number: 460002"s);
    }
}

void Test_4() {
    {
        PhoneNumber phone("+1-2-coursera-cpp"s);
        AssertEqual(phone.GetCountryCode(), "1"s, "+1-2-coursera-cpp: country_code: 1"s);
    }

    {
        PhoneNumber phone("+1-2-coursera-cpp"s);
        AssertEqual(phone.GetCityCode(), "2"s, "+1-2-coursera-cpp: city_code: 2"s);
    }

    {
        PhoneNumber phone("+1-2-coursera-cpp"s);
        AssertEqual(phone.GetLocalNumber(), "coursera-cpp"s, "+1-2-coursera-cpp: local_number: coursera-cpp"s);
    }
}

void Test_5() {
    {
        PhoneNumber phone("1-2-333"s);
        std::cerr << "Something goes wrong: exception should be thrown: \"1-2-333\" hasn't sign plus before number" << std::endl;
    }

    {
        PhoneNumber phone("+7-1233"s);
        std::cerr << "Something goes wrong: exception should be thrown: \"+7-1233\" hasn't local number" << std::endl;
    }
}

void Test_GetInternationalNumber() {
    {
        PhoneNumber phone("+7-495-111-22-33"s);
        AssertEqual(phone.GetInternationalNumber(), "+7-495-111-22-33"s, "intern.number: +7-495-111-22-33"s);
    }

    {
        PhoneNumber phone("+7-495-1112233"s);
        AssertEqual(phone.GetInternationalNumber(), "+7-495-1112233"s, "intern.number: +7-495-1112233"s);
    }

    {
        PhoneNumber phone("+323-22-460002"s);
        AssertEqual(phone.GetInternationalNumber(), "+323-22-460002"s, "intern.number: +323-22-460002"s);
    }

    {
        PhoneNumber phone("+1-2-coursera-cpp"s);
        AssertEqual(phone.GetInternationalNumber(), "+1-2-coursera-cpp"s, "intern.number: +1-2-coursera-cpp"s);
    }
}

// Хочется добиться следующего поведения от unit-тестов:
// 1) Если все тесты отработали успешно, то выполняется основная часть программы.
// 2) Если хотя бы один тест упал, все тесты отрабатывают, но основная часть уже не выполняется.
void TestAll() {
    // Unit-тесты выводят результаты в СТАНДАРТНЫЙ ПОТОК ОШИБОК (std::cerr).
    // Это позволяет нам не комментировать запуск тестов при отправке в тестирующую систему.
    TestRunner tr;
    tr.RunTest(Test_1, "Test_1"s);
    tr.RunTest(Test_2, "Test_2"s);
    tr.RunTest(Test_3, "Test_3"s);
    tr.RunTest(Test_4, "Test_4"s);
    tr.RunTest(Test_5, "Test_5"s);
    tr.RunTest(Test_GetInternationalNumber, "Test_GetInternationalNumber"s);
}  // по этой закрывающей скобке вызывается деструктор "TestRunner::~TestRunner" - для этого мы и используем функцию TestAll