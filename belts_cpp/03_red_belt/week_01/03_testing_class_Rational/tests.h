#pragma once

#include <utility>

#include "rational.h"

void TestConstructor();
void TestParamConstructor_RationalReduction();
void TestNegativeRational();
void TestNegativeRational_If_Both_Negative();
void TestWhenDenominatorIsZero();
void AssertRational(const Rational& submitted, const std::pair<int, int>& correct, const std::string& message);
void TestPredefinedFromStepik();
void TestAll();