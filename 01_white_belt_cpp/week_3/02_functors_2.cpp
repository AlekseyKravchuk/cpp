#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

// ЗАДАЧА: подсчитать количество четных чисел в коллекции, а также сумму этих четных чисел с помощью функтора
template <typename T>
class EvenNumFunctor {
   public:
    void operator()(T value) {
        if (value % 2 == 0) {
            ++_evenCount;
            _evenSum += value;
        }
    }

    void ShowEvenCount() {
        std::cout << "The number of even numbers in collection: " << _evenCount << std::endl;
    }

    void ShowEvenSum() {
        std::cout << "The sum of even numbers in collection: " << _evenSum << std::endl;
    }

   private:
    // переменная для хранения количества четных чисел в коллекции
    T _evenCount{};

    // переменная для хранения суммы четных чисел в коллекции
    T _evenSum{};
};

int main() {
    std::vector<int> v = {2, 1, 3, 9, 11, 8};

    // создаем объект класса "EvenNumFunctor"
    EvenNumFunctor<int> ef;

    for (auto elm : v) {
        ef(elm);
    }

    ef.ShowEvenCount();
    ef.ShowEvenSum();

    return 0;
}
