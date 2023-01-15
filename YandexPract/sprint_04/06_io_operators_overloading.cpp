#include <iostream>
#include <istream>
#include <numeric>
#include <ostream>

using namespace std;

class Rational {
   public:
    Rational() = default;

    Rational(int numerator) : _numerator(numerator), _denominator(1) {}

    Rational(int numerator, int denominator) : _numerator(numerator), _denominator(denominator) { Normalize(); }

    int Numerator() const { return _numerator; }

    int Denominator() const { return _denominator; }

    void SetNumerator(int numerator) { _numerator = numerator; }

    void SetDenominator(int denominator) { _denominator = denominator; }

    void SetFraction(int numerator, int denominator) {
        _numerator = numerator;
        _denominator = denominator;
        Normalize();
    }

    friend ostream& operator<<(ostream& os, Rational fraction);

    friend istream& operator>>(istream& is, Rational& fraction);

   private:
    int _numerator = 0;
    int _denominator = 1;

    void Normalize() {
        if (_denominator < 0) {
            _numerator = -_numerator;
            _denominator = -_denominator;
        }
        const int divisor = gcd(_numerator, _denominator);
        _numerator /= divisor;
        _denominator /= divisor;
    }
};

istream& operator>>(istream& is, Rational& fraction) {
    int numerator, denominator;
    is >> numerator;

    if (is.peek() == '/') {
        is.ignore();
    }

    is >> denominator;

    fraction.SetFraction(numerator, denominator);

    return is;
}

ostream& operator<<(ostream& os, Rational fraction) {
    os << fraction.Numerator() << "/" << fraction.Denominator();
    return os;
}

int main() {
    Rational fract1;
    cin >> fract1;
    cout << "fract1: " << fract1 << endl;
    
    return 0;
}
