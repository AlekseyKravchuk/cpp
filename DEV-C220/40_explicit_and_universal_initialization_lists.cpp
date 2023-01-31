class A {
   private:
    int _x{};
    int _y{};

   public:
    A(int x, int y) : _x(x), _y(y) {}
};

class B : public A {
   private:
    int _x{};
    int _y{};

   public:
    explicit B(int x, int y) : A(x, y), _x(x), _y(y) {}
};

int main() {
    A a1{1, 2};
    B b1{3, 4};
    A a2 = {1, 7};

    // здесь будет ошибка, поскольку в классе "B" конструктор объявлен "explicit"
    B b2 = {9, 22};

    return 0;
}
