#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

/*
Задание по программированию «Обратимая функция»

Реализуйте рассказанный на лекции класс Function, позволяющий создавать, вычислять и инвертировать функцию,
состоящую из следующих элементарных операций:
    - прибавить вещественное число x;
    - вычесть вещественное число x.

При этом необходимо объявить константными все методы, которые по сути такими являются.
 
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
  function.AddPart('-', image.freshness * params.a + params.b);
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
  cout << ComputeQualityByWeight(params, image, 46) << endl;
  return 0;
}

Вывод:
36
20
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

Function MakeWeightFunction(const Params& params, const Image& image) {
    // хотим создать объект класса "Function", который будет использоваться и в "ComputeImageWeight", и в "ComputeQualityByWeight"

    // создаем пустую функцию (объект класса Function)
    Function function;

    // далее описываем эту фукцию, посредством добавления элементарной операции И её аргументов
    function.AddPart('-', image.freshness * params.a + params.b);
    function.AddPart('+', image.rating * params.c);

    return function;
}

// у нас есть некоторый способ вычисления веса изображения "weight" по его качеству "quality"
double ComputeImageWeight(const Params& params, const Image& image) {
    // double weight = image.quality;
    // weight -= image.freshness * params.a + params.b;  // weight = image.quality - (image.freshness * params.a + params.b);
    // weight += image.rating * params.c;                // weight = image.quality - (image.freshness * params.a + params.b) + (image.rating * params.c);

    // return weight;

    Function function = MakeWeightFunction(params, image);

    // когда мы хотим вычислить вес изображения "weight" по его качеству "quality",
    // мы хотим вызвать эту функцию от качества изображения "quality"
    return function.Apply(image.quality);
}

// Обратная функция, позволяющая по весу "weight" вычислять исходное качество "quality".
// Какое у изображения должно было быть качество ("quality"), чтобы у него был вес, передаваемый в качестве параметра?
// Здесь у нас ТАКЖЕ есть некий способ вычисления качества изображения "quality" по его весу "weight" (т.е. почти то же самое, но в обратном порядке)
double ComputeQualityByWeight(const Params& params,
                              const Image& image,
                              double weight) {
    // double quality = weight;

    // // выполняем обратные операции, представленные в "ComputeImageWeight" (в обратном порядке)
    // quality -= image.rating * params.c;
    // quality += image.freshness * params.a + params.b;

    // return quality;

    Function function = MakeWeightFunction(params, image);

    // а здесь нам нужна обратная функция
    // т.е. нужно научить класс "Function" генерировать обратную функцию
    function.Invert();

    // после того, как мы обратили функцию, нужно вызвать её от веса "weight", чтобы получить качество
    return function.Apply(weight);
}

int main() {
    Image image = {10, 2, 6};  // quality = 10; freshness = 2; rating = 6;
    Params params = {4, 2, 6}; // a = 4; b = 2; c = 6
    std::cout << ComputeImageWeight(params, image) << std::endl;
    std::cout << ComputeQualityByWeight(params, image, 46) << std::endl;
    return 0;
}