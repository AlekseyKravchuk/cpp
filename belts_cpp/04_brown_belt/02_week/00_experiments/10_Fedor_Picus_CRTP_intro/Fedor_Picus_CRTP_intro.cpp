#include <iostream>

// базовый класс является шаблоном класса
template <typename D>
class Base {
  public:
    Base() : _i(0) {}

    void f(int i) {
        static_cast<D*>(this)->f(i);
    }

    int get() const {
        return _i;
    }

  protected:
    int _i;
};

// производный класс наследуется от базового, НО! в качестве параметра шаблона использует самого себя.
// Получается, что теперь базовый класс инстанцируется из шаблона в момент наследования от него класса "D",
// => базовый класс Base<D> теперь располагает информацией о производном классе "D"
class D : public Base<D> {
    public:
    void f(int i) {
        _i += i;
        std::cout << "D::f" << std::endl;
    }
};