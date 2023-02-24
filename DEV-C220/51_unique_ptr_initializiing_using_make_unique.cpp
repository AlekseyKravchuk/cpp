#include <iostream>
#include <memory>   // std::unique_ptr, std::make_unique
#include <utility>  // std::move

class A {
   private:
    int _a{};

   public:
    A(int a) : _a(a) {}
    A() = default;

    void SomeMethod() {
        std::cout << "SomeMethod called." << std::endl;
    }
};

int main() {
    std::unique_ptr<A> ptrA1 = std::make_unique<A>(111);

    // std::make_unique<T>() для массива
    int arr_size = 10;  // на этапе выполнения вычисляем размерность создаваемого массива

    // посредством "A[]" вызывается специализация шаблонной функции для массива
    // РЕКОМЕНДУЕМЫЙ способ создания обертки, если НЕ ТРЕБУЕТСЯ пользовательский Deleter
    std::unique_ptr<A[]> uPtrForArray1 = std::make_unique<A[]>(arr_size);

    // можно упростить запись посредством использования "auto":
    auto uPtrForArray2 = std::make_unique<A[]>(arr_size);

    return 0;
} // по этой закрывающей скобке компилятор вызовет деструкторы для всех локальных объектов
