#include <iostream>
#include <istream>
#include <numeric>
#include <ostream>

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

    Rational& operator/=(Rational& rhs) {
        this->SetNumerator(this->Numerator() * rhs.Denominator());
        this->SetDenominator(this->Denominator() * rhs.Numerator());
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
    // Rational r0(1, 2);
    // Rational r1(1, 2);
    // Rational p1(5, 6);
    // r1 -= p1;
    // std::cout << "r1 -= p1 ~ 1/2 - 5/6 = "s << r1 << std::endl;

    // Rational r2(1, 3);
    // Rational p2(4, 5);
    // r2 += p2;
    // std::cout << "r2 += p2 ~ 1/3 + 4/5 = "s << r2 << std::endl;

    // Rational r3(1, 3);
    // std::cout << "-r3 = "s << -r3 << std::endl;

    // cout << "r0 + p1 + r2 + p2 = "s << r1 + p1 + r2 + p2 << endl;

    // Rational k1(1, 8);
    // Rational k2(2, 9);
    // k1 /= k2;
    // cout << "k1 / k2 = "s << k1 << endl;

    // Rational m1(1, 8);
    // Rational m2(2, 9);
    // cout << "m1 / m2 = "s << m1 / m2 << endl;

    // cout << "Введите две обыкновенные дроби в формате x/y:"s << endl;
    // Rational a, b;
    // cin >> a >> b;

    // cout << a << " > "s << b << " ?: "s << boolalpha << (a > b) << endl;
    // cout << a << " < "s << b << " ?: "s << boolalpha << (a < b) << endl;

    Rational j1(4, 5);
    Rational j2(1);
    cout << j1 << " > "s << j2 << " ?: "s << boolalpha << (j1 > j2) << endl;
    cout << j1 << " < "s << j2 << " ?: "s << boolalpha << (j1 < j2) << endl;

    cout << j1 << " >= "s << j2 << " ?: "s << boolalpha << (j1 >= j2) << endl;
    cout << j1 << " <= "s << j2 << " ?: "s << boolalpha << (j1 <= j2) << endl;

    cout << j1 << " == "s << j2 << " ?: "s << boolalpha << (j1 == j2) << endl;

}
