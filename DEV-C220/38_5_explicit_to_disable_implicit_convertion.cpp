class A {
   private:
    int _a{};

   public:
    explicit A(int a) : _a(a) {}
};

int main() {
    // copy-форма вызывает ошибку
    // error: conversion from ‘int’ to non-scalar type ‘A’ requested
    // A a1 = 10;

    // direct-форма работает
    A a2{10};  // OK

    return 0;
}
