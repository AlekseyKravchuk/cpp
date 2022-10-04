#include <iostream>
#include <string>

using namespace std;

class Greeter {
   public:
    Greeter(const string& name) : name_(name) { cout << "Hello, "s << name_ << endl;}
    ~Greeter() {cout << "Goodbye, "s << name_ << endl;}

   private:
    string name_;
};

void Test() {
    Greeter a{"Test-A"s};
    Greeter b{"Test-B"s};
    cout << "Throwing an exception"s << endl;
    throw runtime_error("oops");
}

int main() {
    Greeter greeter_main{"main"s};
    try {
        Greeter outer{"outer"s};

        try {
            Greeter inner{"inner"s};
            Test();
        } catch (const invalid_argument& e) {
            cout << "invalid_argument: "s << e.what() << endl;
        }
        cout << "This text will not be printed"s << endl;
    } catch (const runtime_error& e) {
        cout << "runtime_error: "s << e.what() << endl;
    }
    cout << "------"s << endl;
}