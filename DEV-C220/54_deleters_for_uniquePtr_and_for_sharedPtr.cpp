// Отличие пользовательской delete-функции std::unique_ptr и std::shared_ptr
#include <memory>  // std::shared_ptr<T>

class A {
   public:
    A(int a) : _a(a) {}
    A() : _a(200) {}

    int Get() const {
        return _a;
    }

   private:
    int _a{};
};

// задаем Deleter с помощью лямбда-функции
// очень похож на дефолтовый, но он выполняет некоторый дополнительный полезный функционал (например, логирование)
auto log_and_del = [](A* pA) {
    // некоторые действия по журналированию (логированию)
    delete pA;
};

int main() {
    // TODO: 54:19 Л.5. Ч.2
    // у "std::unique_ptr" deleter является ЧАСТЬЮ ТИПА (тип deleter'a (any callable) указывается вторым параметром шаблона)
    std::unique_ptr<A, decltype(log_and_del)> uPtr(new A, log_and_del);

    // у "std::shared_ptr" ТОЛЬКО ОДИН параметр шаблона,
    // поэтому deleter используется ТОЛЬКО в качестве ПАРАМЕТРА КОНСТРУКТОРА
    std::shared_ptr<A> shPtr(new A, log_and_del);

    return 0;
}
