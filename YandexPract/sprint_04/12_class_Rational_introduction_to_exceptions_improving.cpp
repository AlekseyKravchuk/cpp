#include <iostream>
#include <istream>
#include <numeric>
#include <ostream>
#include <stdexcept>

using namespace std;

class Rational {
   public:
    Rational() = default;

    // конвертирующий конструктор
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

    Rational& operator+=(Rational& rhs) {
        this->SetNumerator(this->Numerator() * rhs.Denominator() + rhs.Numerator() * this->Denominator());
        this->SetDenominator(this->Denominator() * rhs.Denominator());
        this->Normalize();

        return *this;
    }

    Rational& operator-=(Rational& rhs) {
        this->SetNumerator(this->Numerator() * rhs.Denominator() - rhs.Numerator() * this->Denominator());
        this->SetDenominator(this->Denominator() * rhs.Denominator());
        this->Normalize();

        return *this;
    }

    Rational& operator*=(Rational& rhs) {
        this->SetNumerator(this->Numerator() * rhs.Numerator());
        this->SetDenominator(this->Denominator() * rhs.Denominator());
        this->Normalize();

        return *this;
    }

    Rational& operator/=(Rational rhs) {
        this->SetNumerator(this->Numerator() * rhs.Denominator());
        this->SetDenominator(this->Denominator() * rhs.Numerator());
        if (this->Denominator() == 0) {
            throw invalid_argument("denominator of a fraction is zero"s);
        }
        this->Normalize();

        return *this;
    }

    bool operator>(Rational rhs) {
        // this->Normalize();
        // rhs.Normalize();

        int leftNumerator = this->Numerator() * rhs.Denominator();
        int rightNumerator = rhs.Numerator() * this->Denominator();

        return (leftNumerator > rightNumerator) ? true : false;
    }

    bool operator>=(Rational rhs) {
        int leftNumerator = this->Numerator() * rhs.Denominator();
        int rightNumerator = rhs.Numerator() * this->Denominator();

        return (leftNumerator >= rightNumerator) ? true : false;
    }

    bool operator<=(Rational rhs) {
        int leftNumerator = this->Numerator() * rhs.Denominator();
        int rightNumerator = rhs.Numerator() * this->Denominator();

        return (leftNumerator <= rightNumerator) ? true : false;
    }

    bool operator<(Rational rhs) {
        this->Normalize();
        rhs.Normalize();

        int leftNumerator = this->Numerator() * rhs.Denominator();
        int rightNumerator = rhs.Numerator() * this->Denominator();

        return (leftNumerator < rightNumerator) ? true : false;
    }

    friend ostream& operator<<(ostream& os, Rational fraction);
    friend istream& operator>>(istream& is, Rational& fraction);

    friend Rational operator+(Rational lhs, Rational rhs);
    friend Rational operator-(Rational lhs, Rational rhs);

    friend bool operator==(Rational lhs, Rational rhs);
    friend bool operator!=(Rational left, Rational right);

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

Rational operator+(Rational lhs, Rational rhs) {
    return lhs += rhs;
}

Rational operator-(Rational lhs, Rational rhs) {
    return lhs -= rhs;
}

Rational operator/(Rational lhs, Rational rhs) {
    return lhs /= rhs;
}

Rational operator*(Rational lhs, Rational rhs) {
    return lhs *= rhs;
}

// overloading of unary plus
Rational operator+(Rational fraction) { return fraction; }

// overloading of unary minus
Rational operator-(Rational fraction) {
    return {-fraction.Numerator(), fraction.Denominator()};
}

bool operator==(Rational lhs, Rational rhs) {
    lhs.Normalize();
    rhs.Normalize();
    return lhs.Numerator() == rhs.Numerator() &&
           lhs.Denominator() == rhs.Denominator();
}

bool operator!=(Rational left, Rational right) {
    return !(left == right);
}

int main() {
    try {
        const Rational three_fifth{3, 5};
        const Rational zero;
        cout << three_fifth << " / " << zero << " = " << (three_fifth / zero) << endl;
    } catch (const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }
    try {
        Rational value{3, 5};
        value /= Rational();
        // Rational temp = Rational();
        // value /= temp;
        // Следующая строка не должна выполниться
        cout << value << endl;
    } catch (const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }
}