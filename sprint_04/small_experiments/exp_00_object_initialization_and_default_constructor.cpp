using namespace std;

class Rational {
public:
    int Numerator() const {
        return _numerator;
    }

    int Denominator() const {
        return _denominator;
    }

    void SetNumerator(int numerator) {
        _numerator = numerator;
    }

    void SetDenominator(int denominator) {
        if (denominator != 0) {
            _denominator = denominator;
        }
    }

private:
    int _numerator;
    int _denominator;
};