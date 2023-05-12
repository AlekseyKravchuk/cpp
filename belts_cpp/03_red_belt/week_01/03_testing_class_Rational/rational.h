#pragma once

#include <numeric>  // std::gcd
#include <stdexcept>
#include <iostream>

class Rational {
   public:
    Rational() = default;
    Rational(int new_numerator, int new_denominator);
    int Numerator() const;
    int Denominator() const;

   private:
    int _numerator{0};
    int _denominator{1};
};

bool operator==(const Rational& lhs, const Rational& rhs);
bool operator!=(const Rational& lhs, const Rational& rhs);
Rational operator+(const Rational& lhs, const Rational& rhs);
Rational operator-(const Rational& lhs, const Rational& rhs);
Rational operator*(const Rational& lhs, const Rational& rhs);
Rational operator/(const Rational& lhs, const Rational& rhs);
std::istream& operator>>(std::istream& is, Rational& r);
std::ostream& operator<<(std::ostream& os, const Rational& r);
bool operator<(const Rational& lhs, const Rational& rhs);