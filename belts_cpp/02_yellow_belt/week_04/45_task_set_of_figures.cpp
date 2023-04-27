#include <cmath>    // std::sqrt
#include <fstream>  // to redirect input from file
#include <iomanip>  // std::setprecision
#include <iostream>
#include <memory>  // std::shared_ptr<T>, std::make_shared
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// #define _GLIBCXX_DEBUG 1  // включить режим отладки

#ifdef _GLIBCXX_DEBUG
class RedirectStandardInput {
   public:
    RedirectStandardInput(std::ifstream& input) {
        // сохраняем указатель на "streambuf"
        _cinbuf_bkp = std::cin.rdbuf();

        // перенаправляем поток ввода std::cin на файл
        std::cin.rdbuf(input.rdbuf());
    }

    ~RedirectStandardInput() {
        std::cin.rdbuf(_cinbuf_bkp);  // восстанавливаем standard input
    }

   private:
    std::streambuf* _cinbuf_bkp{nullptr};
};
#endif  //_GLIBCXX_DEBUG

/*
================== Набор фигур ==================
Вам дана функция main, которая считывает из стандартного ввода команды по работе с набором геометрических фигур:

int main() {
  vector<shared_ptr<Figure>> figures;
  for (string line; getline(cin, line); ) {
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

Как видно из кода, есть два вида команд:
    ADD — добавить фигуру в набор;
    PRINT — для каждой фигуры в наборе распечатать название, периметр и площадь.

Программа поддерживает три вида фигур: прямоугольник, треугольник и круг. Их добавление описывается так:
    ADD RECT width height — добавить прямоугольник с размерами width и height (например, ADD RECT 2 3).
    ADD TRIANGLE a b c — добавить треугольник со сторонами a, b и c (например, ADD TRIANGLE 3 4 5).
    ADD CIRCLE r — добавить круг радиуса r (например, ADD CIRCLE 5).

Пример работы программы:
Ввод:
ADD RECT 2 3
ADD TRIANGLE 3 4 5
ADD RECT 10 20
ADD CIRCLE 5
PRINT

Вывод:
RECT 10.000 6.000
TRIANGLE 12.000 6.000
RECT 60.000 200.000
CIRCLE 31.400 78.500

Не меняя функцию main, реализуйте недостающие функции и классы:
    - базовый класс Figure с чисто виртуальными методами Name, Perimeter и Area;
    - классы Triangle, Rect и Circle, которые являются наследниками класса Figure и переопределяют его виртуальные методы;
    - функцию CreateFigure, которая в зависимости от содержимого входного потока создаёт shared_ptr<Rect>,
      shared_ptr<Triangle> или shared_ptr<Circle>.

Гарантируется, что все команды ADD корректны; размеры всех фигур — это натуральные числа не больше 1000.
В качестве значения π используйте 3,14.

Как будет тестироваться ваш код:
Автоматическая проверяющая система заменит в вашей программе функцию main на ту, которая приведена в условии задачи,
скомпилирует получившийся файл и прогонит на наборе тестов. Кроме того, отдельно проверяется, что функция CreateFigure
возвращает объект, созданный с помощью make_shared<Rect>, если во входном потоке задан прямоугольник, объект,
созданный с помощью make_shared<Circle>, если во входном потоке задан круг, и т. д.
*/

class Figure {
   public:
    Figure(const std::string type) : _figure_type(type) {}
    virtual const std::string& Name() const = 0;
    virtual double Perimeter() const = 0;
    virtual double Area() const = 0;

   protected:
    const std::string _figure_type{};
};

class Triangle : public Figure {
   public:
    Triangle(std::string type, double a, double b, double c)
        : Figure(type), _a(a), _b(b), _c(c), _p(_a + _b + _c), _half_p(_p / 2) {}

    const std::string& Name() const override {
        return _figure_type;
    }

    double Perimeter() const override { return _p; }

    double Area() const override {
        return std::sqrt(_half_p * (_half_p - _a) * (_half_p - _b) * (_half_p - _c));
    }

   private:
    const double _a{};
    const double _b{};
    const double _c{};
    const double _p{};
    const double _half_p{};
};

class Rect : public Figure {
   public:
    Rect(std::string type, double width, double height)
        : Figure(type), _width(width), _height(height) {}

    const std::string& Name() const override { return _figure_type; }

    double Perimeter() const override { return 2 * (_width + _height); }

    double Area() const override { return _width * _height; }

   private:
    const double _width{};
    const double _height{};
};

class Circle : public Figure {
   public:
    Circle(const std::string type, double r) : Figure(type), _r(r) {}

    const std::string& Name() const override { return _figure_type; }

    double Perimeter() const override { return 2 * _pi * _r; }

    double Area() const override { return _pi * _r * _r; }

   private:
    const double _r{};
    static constexpr double _pi = 3.14;
};

std::shared_ptr<Figure> CreateFigure(std::istringstream& ss) {
    std::string figure_type{};
    ss >> figure_type;
    if (figure_type == "TRIANGLE"s) {
        double a{}, b{}, c{};
        ss >> a >> b >> c;
        return std::make_shared<Triangle>(figure_type, a, b, c);
    } else if (figure_type == "RECT"s) {
        double width{}, height{};
        ss >> width >> height;
        return std::make_shared<Rect>(figure_type, width, height);
    } else if (figure_type == "CIRCLE"s) {
        double r{};
        ss >> r;
        return std::make_shared<Circle>(figure_type, r);
    } else {
        std::cerr << "Unknown figure type"s << std::endl;
        return {};
    }
}

int main() {
#ifdef _GLIBCXX_DEBUG
    // =========== Standard input redirection, debug mode ===========
    std::string path = "42_input.txt"s;
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("File \""s + path + "\" is not opened"s);
    }
    RedirectStandardInput redirection(input);
#endif  //_GLIBCXX_DEBUG
    // =================== End of input redirection ==================

    vector<shared_ptr<Figure>> figures;
    for (string line; getline(cin, line);) {
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