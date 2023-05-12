#include "rational.h"
#include "test_runner.h"
#include "tests.h"


void TestConstructor() {
    {
        Rational r_empty;

        // заменяем вызов шаблонной функции "AssertEqual" на вызов макроса "ASSERT_EQUAL"
        // AssertEqual(r_empty, Rational{0, 1});

        auto r = Rational{0, 1};
        // auto r = Rational{1, 2};  // с ошибкой для проверки работы тестов
        ASSERT_EQUAL(r_empty, r);
    }
}

void TestParamConstructor_RationalReduction() {
    {
        Rational r(4, 6);
        AssertEqual(r, Rational{2, 3});
    }

    {
        Rational r(-6, 8);
        AssertEqual(r, Rational{-3, 4});
    }

    {
        Rational r(6, -8);
        AssertEqual(r, Rational{-3, 4});
    }

    {
        Rational r(-6, -8);
        AssertEqual(r, Rational{3, 4});
    }
}

void TestNegativeRational() {
    {
        Rational r(6, -8);
        AssertEqual(r, Rational{-3, 4});
    }

    {
        Rational r(6, -7);
        AssertEqual(r, Rational{-6, 7});
    }

    {
        Rational r(-6, -7);
        AssertEqual(r, Rational{6, 7});
    }
}

void TestNegativeRational_If_Both_Negative() {
    {
        Rational r(-6, -8);
        AssertEqual(r, Rational{3, 4});
    }
}

void TestWhenDenominatorIsZero() {
    {
        Rational r(0, -8);
        AssertEqual(r, Rational{0, 1});
    }
}

void AssertRational(const Rational& submitted, const std::pair<int, int>& correct, const std::string& message) {
    AssertEqual(std::pair<int, int>(submitted.Numerator(), submitted.Denominator()), correct, message);
}

const auto p = [](int numerator, int denominator) {
    return std::pair<int, int>(numerator, denominator);
};

void TestPredefinedFromStepik() {
    AssertRational(Rational(1, 1), p(1, 1), "Canonical form of 1/1 is 1/1");
    AssertRational(Rational(3, 5), p(3, 5), "Canonical form of 3/5 is 3/5");
    AssertRational(Rational(2147483647, 2147483647), p(1, 1), "Canonical form of 2147483647/2147483647 is 1/1");
    AssertRational(Rational(-5, 4), p(-5, 4), "Canonical form of -5/4 is -5/4");
    AssertRational(Rational(5, -4), p(-5, 4), "Canonical form of 5/-4 is -5/4");
    AssertRational(Rational(-6, -2), p(3, 1), "Canonical form of -6/-2 is 3/1");
    AssertRational(Rational(21, 56), p(3, 8), "Canonical form of 21/56 is 3/8");
    AssertRational(Rational(0, 100), p(0, 1), "Canonical form of 0/100 is 0/1");
    AssertRational(Rational(), p(0, 1), "Canonical form of default constructed is 0/1");
}

void TestAll() {
    // Unit-тесты выводят результаты в СТАНДАРТНЫЙ ПОТОК ОШИБОК (std::cerr).
    // Это позволяет нам не комментировать запуск тестов при отправке в тестирующую систему.
    TestRunner tr;

    // заменяем вызов метода "RunTest" объекта "tr" на вызов макроса
    // tr.RunTest(TestConstructor, "TestConstructor"s);
    RUN_TEST(tr, TestConstructor);

    // tr.RunTest(TestParamConstructor_RationalReduction, "TestParamConstructor_RationalReduction"s);
    RUN_TEST(tr, TestParamConstructor_RationalReduction);

    // tr.RunTest(TestNegativeRational, "TestNegativeRational"s);
    RUN_TEST(tr, TestNegativeRational);

    // tr.RunTest(TestNegativeRational_If_Both_Negative, "TestNegativeRational_If_Both_Negative"s);
    RUN_TEST(tr, TestNegativeRational_If_Both_Negative);

    // tr.RunTest(TestWhenDenominatorIsZero, "TestWhenDenominatorIsZero"s);
    RUN_TEST(tr, TestWhenDenominatorIsZero);

    // tr.RunTest(TestPredefinedFromStepik, "TestPredefinedFromStepik");
    RUN_TEST(tr, TestPredefinedFromStepik);

}  // по этой закрывающей скобке вызывается деструктор "TestRunner::~TestRunner" - для этого мы и используем функцию TestAll
