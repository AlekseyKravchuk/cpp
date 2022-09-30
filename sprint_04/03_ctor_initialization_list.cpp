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

int main() {
    

    return 0;
}