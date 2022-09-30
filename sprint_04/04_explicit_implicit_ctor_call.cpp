#include <iostream>
#include <numeric>

using namespace std;

class Rational {
   public:
    Rational() = default;

    Rational(int numerator) : _numerator(numerator), _denominator(1) {}

    Rational(int numerator, int denominator) : _numerator(numerator), _denominator(denominator) {
        Normalize();
    }

    int Numerator() const {
        return _numerator;
    }

    int Denominator() const {
        return _denominator;
    }

   private:
    void Normalize() {
        if (_denominator < 0) {
            _numerator = -_numerator;
            _denominator = -_denominator;
        }
        const int divisor = gcd(_numerator, _denominator);
        _numerator /= divisor;
        _denominator /= divisor;
    }

    int _numerator = 0;
    int _denominator = 1;
};

Rational AddRationals(Rational r1, Rational r2) {
    int numerator = r1.Numerator() * r2.Denominator() + r2.Numerator() * r1.Denominator();
    int denominator = r1.Denominator() * r2.Denominator();

    return {numerator, denominator};
}

int main() {
    Rational sum = AddRationals(Rational{1, 6}, 5);
    // выведет 31/6
    cout << sum.Numerator() << "/"s << sum.GetDenominator() << endl;
}