// #include <algorithm>
#include <iostream>
#include <string>

using namespace std::literals;

/* 
В стандартном потоке даны три строки, разделённые пробелом.
Каждая строка состоит из строчных латинских букв и имеет длину не более 30 символов.
Выведите в стандартный вывод лексикографически минимальную из них.
 */

int main() {
    std::string s1{}, s2{}, s3{};

    std::cin >> s1 >> s2 >> s3;
    s1 = s1 < s2 ? s1 : s2;
    s3 = s1 < s3 ? s1 : s3;
    std::cout << s3/*  << std::endl */;

    // std::cout << std::min({s1, s2, s3}) /* << std::endl */;

    return 0;
}
