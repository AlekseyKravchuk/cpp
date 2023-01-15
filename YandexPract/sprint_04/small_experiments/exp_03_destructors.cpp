#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Greeter {
   public:
    Greeter(const string& name)
        : name_(name) {
        cout << "Hello, "s << name_ << "!"s << endl;
    }

    ~Greeter() {
        // Поля класса внутри деструктора еще не уничтожены, и к ним можно обращаться
        cout << "Goodbye, "s << name_ << "!"s << endl;
    }

   private:
    string name_;
};

int main() {
    {
        Greeter g("Aleks"s);
    }
    cout << "In main" << endl;
    
/*     vector<string> names = {
        "Ivan"s,
        "Vladimir"s,
    };
    for (const string& name : names) {
        cout << "Begin"s << endl;
        Greeter greeter(name);
        cout << "End"s << endl;
    } */
}