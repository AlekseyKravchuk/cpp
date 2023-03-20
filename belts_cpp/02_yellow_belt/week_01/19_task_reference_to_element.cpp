#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

using namespace std::literals;

/*
Реализуйте шаблонную функцию GetRefStrict, которая на вход принимает: map и значение ключа k.
Если элемент по ключу k в коллекции отсутствует, то функция должна бросить исключение runtime_error, иначе вернуть ссылку на элемент в коллекции.

Пример использования функции\

map<int, string> m = {{0, "value"}};
string& item = GetRefStrict(m, 0);
item = "newvalue";
cout << m[0] << endl; // выведет newvalue
*/

template <typename KeyType, typename ValueType>
ValueType& GetRefStrict(std::map<KeyType, ValueType>& m, KeyType key) {
    if (m.count(key) == 0) {
        throw std::runtime_error("Key is not found in dictionary"s);
    }
    return m.at(key);
}

int main() {
    // std::map<int, std::vector<double>> m{
    //     {1, {1.1, 1.11, 1.111}},
    //     {2, {2.2, 2.22, 2.222}},
    //     {3, {3.3, 3.33, 3.333}}};

    // int key2search = 3;
    // auto res = GetRefStrict(m, key2search);

    std::map<int, std::string> m = {{0, "value"}};
    std::string& item = GetRefStrict(m, 0);
    item = "newvalue";
    std::cout << m[0] << std::endl;  // выведет newvalue

    return 0;
}
