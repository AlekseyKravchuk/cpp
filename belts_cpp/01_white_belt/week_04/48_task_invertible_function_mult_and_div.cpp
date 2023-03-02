#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

/*
Задание по программированию
«Обратимая функция с умножением и делением»

Добавьте в класс Function из задачи «Обратимая функция» обработку умножения ('*') и деления ('/'). Гарантируется отсутствие элементарных операций умножения и деления на 0.
 
Пример
Код:

struct Image {
  double quality;
  double freshness;
  double rating;
};

struct Params {
  double a;
  double b;
  double c;
};

Function MakeWeightFunction(const Params& params,
                            const Image& image) {
  Function function;
  function.AddPart('*', params.a);
  function.AddPart('-', image.freshness * params.b);
  function.AddPart('+', image.rating * params.c);
  return function;
}

double ComputeImageWeight(const Params& params, const Image& image) {
  Function function = MakeWeightFunction(params, image);
  return function.Apply(image.quality);
}

double ComputeQualityByWeight(const Params& params,
                              const Image& image,
                              double weight) {
  Function function = MakeWeightFunction(params, image);
  function.Invert();
  return function.Apply(weight);
}

int main() {
  Image image = {10, 2, 6};
  Params params = {4, 2, 6};
  cout << ComputeImageWeight(params, image) << endl;
  cout << ComputeQualityByWeight(params, image, 52) << endl;
  return 0;
}

Вывод:
72
5
*/

struct Image {
    double quality;
    double freshness;
    double rating;
};

struct Params {
    double a;
    double b;
    double c;
};

class FunctionPart {
   public:
    FunctionPart(char new_operation, double new_operand) : _operation(new_operation), _operand(new_operand) {}

    // Метод "Apply" будет применять операцию "new_operation" к новому введенному числу "source_value" следующим образом:
    // source_value = [source_value] [_operation("+" или -"")] [_operand]
    double Apply(double source_value) const {
        switch (_operation) {
            case '+':
                source_value += _operand;
                break;
            case '-':
                source_value -= _operand;
                break;
            case '*':
                source_value *= _operand;
                break;
            case '/':
                source_value /= _operand;
                break;
            default:
                std::cout << "Unsupported operation: '" << _operation << "'." << std::endl;
        }

        return source_value;
    }

    void Invert() {
        switch (_operation) {
            case '+':
                _operation = '-';
                break;
            case '-':
                _operation = '+';
                break;
            case '*':
                _operation = '/';
                break;
            case '/':
                _operation = '*';
                break;
            default:
                std::cout << "Unsupported operation: '" << _operation << "'." << std::endl;
        }
    }

   private:
    char _operation;
    double _operand;
};

class Function {
   public:
    // конструктор не писали, т.к. нам достаточно того конструктора, который компилятор сгенерирует сам - сделает вектор "_parts" пустым

    // метод "AddPart" будет добавлять очередную часть в нашу функцию
    void AddPart(char operation, double operand) {
        _parts.push_back({operation, operand});
    }

    // чтобы применить конкретную функцию к введенному значению "value",
    // нужно последовательно применить к этому значению все элементарные операции
    double Apply(double value) const {
        for (const FunctionPart& part : _parts) {
            value = part.Apply(value);
        }

        return value;
    }

    void Invert() {
        // чтобы инвертировать саму функцию, нужно инвертировать каждую её часть.
        for (FunctionPart& part : _parts) {
            part.Invert();
        }

        // кроме инвертирования каждой части, их порядок следования в "_parts" нужно изменить на противоположный
        std::reverse(_parts.begin(), _parts.end());
    }

   private:
    // поле для хранения элементарных операций
    std::vector<FunctionPart> _parts;
};

Function MakeWeightFunction(const Params& params,
                            const Image& image) {
    Function function;
    function.AddPart('*', params.a);
    function.AddPart('-', image.freshness * params.b);
    function.AddPart('+', image.rating * params.c);
    return function;
}

double ComputeImageWeight(const Params& params, const Image& image) {
    Function function = MakeWeightFunction(params, image);
    return function.Apply(image.quality);
}

double ComputeQualityByWeight(const Params& params,
                              const Image& image,
                              double weight) {
    Function function = MakeWeightFunction(params, image);
    function.Invert();
    return function.Apply(weight);
}

int main() {
    Image image = {10, 2, 6};
    Params params = {4, 2, 6};
    std::cout << ComputeImageWeight(params, image) << std::endl;
    std::cout << ComputeQualityByWeight(params, image, 52) << std::endl;
    return 0;
}