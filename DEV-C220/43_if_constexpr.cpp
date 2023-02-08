#include <iostream>
#include <string>
#include <type_traits>
#include <utility>  // std::exchange

using namespace std::literals;

// Есть некая шаблонная функция, которая в качестве параметра принимает
// ИЛИ значение типа "T" ИЛИ указатель на тип "T",
// а возвращать из этой функции всегда хотелось бы ЗНАЧЕНИЕ
// template <typename T>
// const T GetValue(const T t) {
//     return t;
// }

// пробуем перегрузить фунцию для получения желаемого результата ==> НЕТ ЭФФЕКТА
// template <typename T>
// const T& GetValue(const T* t_ptr) {
//     std::cout << "\"GetValue(const T* t_ptr)\" called." << std::endl;
//     return *t_ptr;
// }

// пробуем решить задачу с использованием "type_traits"
// template <typename T>
// const T GetValue(const T t) {
//     // пробуем использовать обычный "if", т.е. условные конструкции будут выполняться во время выполнения!!!
//     if (std::is_pointer_v<T>) {
//         return *t; // ЗДЕСЬ компилятор выдаст ошибку
//     } else {
//         return t;
//     }
// }

template <typename T>
auto GetValue(const T t) {
    // пробуем использовать обычный "if", т.е. условные конструкции будут выполняться во время выполнения!!!
    if constexpr (std::is_pointer_v<T>) {
        return *t; // ЗДЕСЬ компилятор выдаст ошибку
    } else {
        return t;
    }
}

int main() {
    int n = 10;
    int n_ref = n;
    std::cout << GetValue(n) << std::endl;      // value:   10
    std::cout << GetValue(&n) << std::endl;     // !!! pointer: 0x7fffffffda50, а хотелось бы получить ЗНАЧЕНИЕ
    std::cout << GetValue(n_ref) << std::endl;  // value:   10

    return 0;
}