#include <functional>
#include <iostream>
#include <string>
#include <utility>  // std::move, std::as_const
using namespace std;

// 3. Capture using move-semantics

struct MyStruct {
    int field = 5;

    auto get_callable(int i) const {
        /* static */ std::string s = "abcde";  // без "static" при захвате по константной ссылке будет UB

        // начиная с С++14 мы можем захватывать в лямбда-функцию с помощью std::move
        auto f = [s = std::move(s), i]() {
            cout << "i = " << i
                << ", s = \"" << s
                << "\", s[" << i << "] = " << s[i]
                << endl;
        };

        // // начиная с С++14 мы можем также захватывать в лямбда-функцию по КОНСТАНТНОЙ ссылке?
        // // !!! но это чревато UB !!!
        // auto f = [&s = std::as_const(s), i]() {
        //     cout << "i = " << i
        //          << ", s = \"" << s
        //          << "\", s[" << i << "] = " << s[i]
        //          << endl;
        // };

        return f;
    }
};

int main() {
    {
        MyStruct obj;
        auto callable_1 = obj.get_callable(1);

        // auto callable_1 = MyStruct().get_callable(1);

        cout << "size(callable_1) = " << sizeof(callable_1) << endl;
        callable_1();
    }
    // auto callable_1 = MyStruct().get_callable();
    // auto callable_2 = MyStruct().get_callable();
    // auto callable_3 = MyStruct().get_callable();

    // int res_1 = callable_1(20);    // OK
    // int res_2 = callable_2(200);   // OK
    // int res_3 = callable_3(7000);  // OK

    // cout << "callable(20) = " << res_1 << endl;
    // cout << "callable(200) = " << res_2 << endl;
    // cout << "callable(7000) = " << res_3 << endl;

    return 0;
}
