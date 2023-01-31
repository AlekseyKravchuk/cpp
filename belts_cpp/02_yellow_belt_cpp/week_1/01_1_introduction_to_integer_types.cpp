#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

using namespace std::literals;

int main() {
    // Пусть требуется найти среднее арифметическое показаний температуры за 3 дня.
    // Если все значения положительные (неотрицательные), то программа работает как положено.
    // std::vector<int> v {8, 7, 3};

    // Но если в вектора имеются отрицательные значения, код работать не будет
    std::vector<int> v{-8, -7, 3};

    int sum{0};

    for (auto i : v) {
        sum += i;
    }

    // "v.size()" возвращает значение типа "size_t" (БЕЗЗНАКОВЫЙ целый тип => отсюда и проблема с результатом).
    // размер типа "size_t" зависит от разрядности ОС - если 64 бита, то и размер типа "size_t" будет равен 64 бита
    int avg = sum / v.size();

    static_assert(std::is_same_v<size_t, decltype(sum / v.size())>);

    // int avg = sum / static_cast<int>(v.size());

    std::cout << "avg = " << avg << std::endl;

    return 0;
}
