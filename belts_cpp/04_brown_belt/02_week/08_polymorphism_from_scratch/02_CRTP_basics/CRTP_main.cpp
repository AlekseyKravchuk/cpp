#include <iostream>

// Метапрограммирование 5. ADL и CRTP:
// https://www.youtube.com/watch?v=FmOs-yM5Cgc
// 
// Метапрограммирование (4 курс, осень 2023) - Санду Р. А.
// https://www.youtube.com/playlist?list=PL4_hYwCyhAvYO01i2gR-prnu4Stvxuf7u

// Примечание: поле типа "T" в шаблонном классе "Base" не получится
// НО тип "T" в методах использовать можно благодаря "ленивой" инстанциации методов
template <typename T>
struct Base {
    // В момент, когда мы будем вызывать методы "interface()" и "static_func()" у нас уже будет экземпляр класса "Derived"
    // 
    void interface() {
        static_cast<T *>(this)->implementation();
    }

    static void static_func() {
        T::static_sub_func();
    }

    // если в этом классе завести поле с типом "T", то эта конструкция перестанет работать
    // T some_value;  // error: ‘Base<T>::some_value’ has incomplete type
};

// Это работает поскольку на момент наследования мы инстанцируем шаблонный класс "Base", но не его методы
// Методы инстанцируются "лениво", т.е. по мере необходимости, а в самом шаблоне "Base" кроме этих методов ничего нет
// В момент инстанцирования класса "Base" тип "T" (он же "Derived") это "incomplete type"

// сначала инстанцируется сам класс в момент наследования без всяких методов
// "Ленивая" инстанциация: класс инстанцируется, но инстанциация методов отложена
struct Derived : public Base<Derived> {
    void implementation();

    static void static_sub_func();
};

int main() {
    return 0;
}
