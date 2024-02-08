#include <functional>
#include <iostream>

using namespace std;

// 2. Default-capture and capture with initialization (starting from C++14)

struct MyStruct {
    int field = 5;

    auto get_callable() const {
        // захватываем поле "field" класса "MyStruct" через список инициализации
        // фактически в [] мы описываем список инициализации для конструктора функционального объекта
        auto f = [field_copy = this->field](int x) -> int {
            return x + field_copy;
        };

        return f;
    }
};

int main() {
    auto callable_1 = MyStruct().get_callable();
    auto callable_2 = MyStruct().get_callable();
    auto callable_3 = MyStruct().get_callable();

    int res_1 = callable_1(20);    // OK
    int res_2 = callable_2(200);   // OK
    int res_3 = callable_3(7000);  // OK

    cout << "callable(20) = " << res_1 << endl;
    cout << "callable(200) = " << res_2 << endl;
    cout << "callable(7000) = " << res_3 << endl;

    return 0;
}
