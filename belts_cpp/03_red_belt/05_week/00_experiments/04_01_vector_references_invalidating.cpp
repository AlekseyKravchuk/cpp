#include <iostream>
#include <vector>

using namespace std::literals;

int main() {
    std::vector<int> v = {1, 2, 3};

    // запоминаем ссылку на первый элемент вектора
    int& first = v[0];

    std::cout << "first element = "s << first << "\n";

    v.push_back(4);  // в этом месте происходит инвалидация ссылки на первый элемент вектора (ссылка перестает быть валидной)

    std::cout << "first element after push_back() = "s << first << "\n";

    return 0;
}
