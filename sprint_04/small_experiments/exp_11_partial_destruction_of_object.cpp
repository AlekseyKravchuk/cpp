#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

class Greeter {
   public:
    Greeter(const string& name)
        : name_(name) {
        cout << "Hello, "s << name_ << endl;
    }

    ~Greeter() {
        cout << "Goodbye, "s << name_ << endl;
    }

   private:
    string name_;
};

class Thrower {
   public:
    Thrower() : field_("field"s) {
        cout << "Thrower()"s << endl;
        // Исключение, выброшенное здесь, будет поймано за пределами конструктора
        throw runtime_error("Oops"s);
    }

    ~Thrower() {
        cout << "~Thrower()"s << endl;
    }

   private:
    Greeter field_;
};

int main() {
    try {
        Thrower t;
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    }
}