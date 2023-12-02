#include <deque>
#include <iostream>
#include <vector>

using namespace std::literals;

int main() {
    std::cout << "=========== vector ==========="s << '\n';
    {
        std::vector<int> numbers = {1};
        auto it_vector = numbers.begin();

        std::cout << *it_vector << std::endl;

        for (int i = 100; i < 10'000; ++i) {
            numbers.push_back(i);
        }

        std::cout << *it_vector << std::endl;
    }
    std::cout << "=============================="s << '\n';

    std::cout << "=========== deque ==========="s << '\n';
    {
        std::deque<int> numbers = {1};
        auto it_deque = numbers.begin();

        std::cout << *it_deque << std::endl;

        for (int i = 100; i < 10'000; ++i) {
            numbers.push_back(i);
        }

        std::cout << *it_deque << std::endl;

        // хотим с помощью имеющегося итератора посмотреть на последний элемент дека
        // std::cout << *(std::begin(numbers) + numbers.size() - 1) << std::endl;

        // дек инвалидирует итераторы, при этом сохраняет указатели и ссылки валидными
        std::cout << *(it_deque + numbers.size() - 1) << std::endl;
    }
    std::cout << "=============================="s << '\n';

    return 0;
}