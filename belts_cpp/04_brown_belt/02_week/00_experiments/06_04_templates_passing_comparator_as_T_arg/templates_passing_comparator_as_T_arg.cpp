#include <cstddef> // size_t
#include <cstring> // std::strcmp
#include <iostream>
#include <string>

using namespace std;

// В качестве "Comparator" можно использовать все, что угодно, у чего можно вызвать оператор "operator()"
// т.е. в качестве "Comparator" м.б. передан ФУНКТОР, лямбду, обычную функцию
template <typename T, typename Comparator>
void sort(T* begin, T* end, Comparator cmp) {
    auto mid = (end - begin) / 2;
    if (cmp(*begin, *mid)) {
        // some code to reorder container
    }
}

// пишем собственный ФУНКТОР, т.е. класс (структуру), у которого перегружен оператор "operator()"
struct istr_cmp {
    // перегружаем оператор "operator()
    bool operator()(const string& lhs, const string& rhs) const {
        return strcmp(lhs.c_str(), rhs.c_str()) < 0;
    }
};

int main() {
    string names[10] = {/* */};

    // аналог "std::end()" для массива - это "начало массива + количество элементов"
    // "istr_cmp()" - это объект типа "Comparator"
    // Есть стандартный компаратор "std::less". Он передается по умолчанию в такие контейнеры, как std::map, std::set и т.д.
    sort(names, names + 10, istr_cmp());

    return 0;
}
