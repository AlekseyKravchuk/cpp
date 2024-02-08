#include <functional>
#include <iostream>

using namespace std;

// 1. Capture of this

struct MyStruct {
    int field = 5;

    auto get_callable() const {
        // локальные переменные можно захватывать в лямбде, если мы их захватим, то они будут видны
        // но "field" - это не локальная переменная
        // и обычный синтаксис захвата локальной переменной не позволяет нам использовать "field" в лямбде
        // для доступа к полям класса из лямбды нужно захватить "this" (указатель на текущий объект)

        // здесь нужно быть уверенным, что лямбда не переживет текущий объект "MyStruct", другими словами, 
        // нужно быть уверенным, что объект не уничтожится раньше, чем callable-объект, возвращаемый методом "get_callable"
        auto f = [this](int x) -> int {
            return x + field;
        };

        return f;
    }
};

int main() {
    // MyStruct obj;
    // obj.method();

    // !!! Потенциально опасный код - Undefined Behaviour (UB) !!!

    auto callable_1 = MyStruct().get_callable();
    auto callable_2 = MyStruct().get_callable();
    auto callable_3 = MyStruct().get_callable();

    // UB: анонимный объект "MyStruct()" в этом месте уже не существует,
    // а "callable" внутри себя обращается к полю "field" уже несуществующего объекта по невалидному указателю "this"
    int res_1 = callable_1(20);    // UB
    int res_2 = callable_2(200);   // UB
    int res_3 = callable_3(7000);  // UB

    cout << "callable(20) = " << res_1 << endl;
    cout << "callable(200) = " << res_2 << endl;
    cout << "callable(7000) = " << res_3 << endl;

    return 0;
}
