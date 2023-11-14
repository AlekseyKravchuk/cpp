#include <iostream>
#include <string>

using namespace std::literals;

int main() {
    // после удаления ненужных символов хочется получить строку "ab cd"
    std::string test_str = "   ab cd    "s;

    const std::string chars_to_skip = " "s;

    // trim leading whitespaces
    size_t pos = test_str.find_first_not_of(chars_to_skip);
    if (pos != test_str.npos) {
        test_str.erase(0, pos);
    }

    // trim trailing whitespaces
    pos = test_str.find_last_not_of(chars_to_skip);  // будет возвращена позиция, указывающая на букву 'd'
    std::cout << "pos = "s << pos << std::endl;      // выводим значение позиции
    ++pos;                                           // сдвигаемся на одну позицию вправо, т.е. на первый из пробелов после слова

    test_str.erase(pos);

    std::cout << "test_str = "s << test_str << std::endl;

    return 0;
}
