#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

/*
Дана строка. Найдите в этой строке второе вхождение буквы f и выведите индекс этого вхождения.
Если буква f в данной строке встречается только один раз, выведите число -1, а если не встречается ни разу, выведите число -2.
Индексы нумеруются с нуля.
 */

int FindIndexOfNthOccurrence(std::string s, char ch, size_t num_of_occur) {
    size_t occurred = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == ch) {
            ++occurred;
            if (occurred == num_of_occur) {
                return i;
            }
        }
    }
    return occurred > 0 ? -1 : -2;
}

int main() {
    std::string s;
    std::cin >> s;

    std::cout << FindIndexOfNthOccurrence(s, 'f', 2) <<  std::endl;

    return 0;
}
