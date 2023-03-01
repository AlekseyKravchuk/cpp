#include <iostream>
#include <map>
#include <numeric>  // std::gcd
#include <set>
#include <sstream>
#include <stdexcept>  // std::domain_error
#include <string>
#include <vector>

/*
Часть 5
Наконец, вам нужно реализовать возможность использовать объекты класса Rational в качестве элементов контейнера set и ключей в контейнере map.
Пример:

set<Rational> rationals;
rationals.insert(Rational(1, 2));
rationals.insert(Rational(1, 3));

map<Rational, string> name;
name[Rational(1, 2)] = "одна вторая";

*/

class Rational {
   public:
    Rational() = default;

    Rational(int new_numerator, int new_denominator) {
        int gcd = std::gcd(new_numerator, new_denominator);
        _numerator = new_numerator / gcd;
        _denominator = new_denominator / gcd;

        if (_denominator < 0) {
            _numerator = -_numerator;
            _denominator = -_denominator;
        }
    }

    int Numerator() const {
        return _numerator;
    }

    int Denominator() const {
        return _denominator;
    }

   private:
    int _numerator{0};
    int _denominator{1};
};

bool operator==(const Rational& lhs, const Rational& rhs) {
    return (lhs.Numerator() == rhs.Numerator()) &&
           (lhs.Denominator() == rhs.Denominator());
}

Rational operator+(const Rational& lhs, const Rational& rhs) {
    return {
        lhs.Numerator() * rhs.Denominator() + rhs.Numerator() * lhs.Denominator(),
        lhs.Denominator() * rhs.Denominator()};
}

Rational operator-(const Rational& lhs, const Rational& rhs) {
    return {
        lhs.Numerator() * rhs.Denominator() - rhs.Numerator() * lhs.Denominator(),
        lhs.Denominator() * rhs.Denominator()};
}

Rational operator*(const Rational& lhs, const Rational& rhs) {
    return {lhs.Numerator() * rhs.Numerator(), lhs.Denominator() * rhs.Denominator()};
}

Rational operator/(const Rational& lhs, const Rational& rhs) {
    if (!rhs.Denominator()) {
        throw std::domain_error("Division by zero");
    }

    return lhs * Rational(rhs.Denominator(), rhs.Numerator());
}

std::istream& operator>>(std::istream& is, Rational& r) {
    int n{}, d{};
    char delim{};

    if (is >> n >> delim >> d && delim == '/') {
        r = Rational(n, d);
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const Rational& r) {
    os << r.Numerator() << '/' << r.Denominator();

    return os;
}

bool operator<(const Rational& lhs, const Rational& rhs) {
    return (lhs.Numerator() * rhs.Denominator()) < (rhs.Numerator() * lhs.Denominator());
}

int main() {
    {
        const std::set<Rational> rs = {{1, 2}, {1, 25}, {3, 4}, {3, 4}, {1, 2}};
        if (rs.size() != 3) {
            std::cout << "Wrong amount of items in the set" << std::endl;
            return 1;
        }

        std::vector<Rational> v;
        for (auto x : rs) {
            v.push_back(x);
        }
        if (v != std::vector<Rational>{{1, 25}, {1, 2}, {3, 4}}) {
            std::cout << "Rationals comparison works incorrectly" << std::endl;
            return 2;
        }
    }

    {
        std::map<Rational, int> count;
        ++count[{1, 2}];
        ++count[{1, 2}];

        ++count[{2, 3}];

        if (count.size() != 2) {
            std::cout << "Wrong amount of items in the map" << std::endl;
            return 3;
        }
    }

    std::cout << "OK" << std::endl;
    return 0;
}