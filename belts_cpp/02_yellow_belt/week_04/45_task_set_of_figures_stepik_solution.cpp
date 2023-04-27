#include <cmath>
#include <exception>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class Figure {
   public:
    virtual std::string Name() const = 0;
    virtual double Perimeter() const = 0;
    virtual double Area() const = 0;
};

class Rect : public Figure {
    double const width = 0.0;
    double const height = 0.0;

   public:
    Rect(double w, double h) : width(w), height(h) { }

    virtual std::string Name() const override { return "RECT"; }

    virtual double Perimeter() const override { return 2.0 * (width + height); }

    virtual double Area() const override { return width * height; }
};

class Circle : public Figure {
    inline static constexpr double PI = 3.14;
    double const radius = 0.0;

   public:
    Circle(double r) : radius(r) { }

    virtual std::string Name() const override { return "CIRCLE"; }

    virtual double Perimeter() const override { return 2.0 * PI * radius; }

    virtual double Area() const override { return PI * radius * radius; }
};

class Triangle : public Figure {
    double const A = 0.0;
    double const B = 0.0;
    double const C = 0.0;

   public:
    Triangle(double a, double b, double c) : A(a), B(b), C(c) { }

    virtual std::string Name() const override { return "TRIANGLE"; }

    virtual double Perimeter() const override { return A + B + C; }

    virtual double Area() const override {
        auto p = this->Perimeter() / 2.0;
        auto area = p * (p - A) * (p - B) * (p - C);
        return std::sqrt(area);
    }
};

std::shared_ptr<Figure> CreateFigure(std::istream& input) {
    std::string figure;
    input >> figure;
    if (figure == "RECT") {
        double width, heigth;
        input >> width >> heigth;
        return std::make_shared<Rect>(width, heigth);
    }
    if (figure == "TRIANGLE") {
        double a, b, c;
        input >> a >> b >> c;
        return std::make_shared<Triangle>(a, b, c);
    }
    if (figure == "CIRCLE") {
        double radius;
        input >> radius;
        return std::make_shared<Circle>(radius);
    }
    throw std::invalid_argument("wrong type figure");
}

std::string test{R"(
ADD RECT 2 3
ADD TRIANGLE 3 4 5
ADD RECT 10 20
ADD CIRCLE 5
PRINT
)"};

std::istringstream input(test);
using namespace std;

int main() {
    vector<shared_ptr<Figure>> figures;
    for (string line; getline(input, line);) {
        istringstream is(line);

        string command;
        is >> command;
        if (command == "ADD") {
            figures.push_back(CreateFigure(is));
        } else if (command == "PRINT") {
            for (const auto& current_figure : figures) {
                cout << fixed << setprecision(3)
                     << current_figure->Name() << " "
                     << current_figure->Perimeter() << " "
                     << current_figure->Area() << endl;
            }
        }
    }
    return 0;
}