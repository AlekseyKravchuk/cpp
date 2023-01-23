class A {
   public:
    explicit A(int a) : _a(a) {}
    A(double) = delete;

   private:
    int _a{};
};

int main() {
    A a1(15);

    // ключевое слово "explicit" при объявлении конструктора спасает только от таких ситуаций:
    // A a2_1 = 19;  // Благодаря наличию "explicit" получаем ERROR: conversion from ‘int’ to non-scalar type ‘A’ requested
    // A a2_2 = 19.1;

    // Но по-прежнему здесь компилятор выполнить неявное преобразование (приведение) типов: "double" -> "int"
    // A a3(18.9);
    // A a4(20.9f);
    // A a5(20.9L);  // неоднозначность "long double" -> "double" ИЛИ "long double" -> "int"

    // А хотелось бы ЗАПРЕТИТЬ неявное преобразование (приведение) типов: "double" -> "int"
    // Решение проблемы: "A(double) = delete;"

    return 0;
}
