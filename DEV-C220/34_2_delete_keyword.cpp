class A {
   public:
    explicit A(long long a) : _a(a) {}
    A(long) = delete;

   private:
    long long _a{};
};

int main() {
    // A a1('a'); // ERROR
    // A a2(15);  // ERROR
    A a3(19LL);   // OK

    return 0;
}
