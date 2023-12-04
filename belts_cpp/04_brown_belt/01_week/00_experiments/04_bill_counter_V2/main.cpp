#include <array>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

// Задача: написать счетчик купюр - хотим уметь считать сколько купюр каждого из номиналов у нас есть в наличии

using Nominal = int;

// ===================================================================================
template <typename Collection>
std::string Join(const Collection& collection, const std::string& separator = " ") {
    std::ostringstream oss;
    bool isFirst = true;

    for (const auto& elm : collection) {
        if (!isFirst) {
            oss << separator;
        }

        isFirst = false;
        oss << elm;
    }

    return oss.str();
}
// ===================================================================================

class RedirectStandardInput {
  public:
    RedirectStandardInput(std::ifstream& input) {
        // сохраняем указатель на "streambuf"
        _cinbuf_bkp = std::cin.rdbuf();

        // перенаправляем поток ввода std::cin на файл
        std::cin.rdbuf(input.rdbuf());
    }

    ~RedirectStandardInput() {
        std::cin.rdbuf(_cinbuf_bkp); // восстанавливаем standard input
    }

  private:
    std::streambuf* _cinbuf_bkp{nullptr};
};
// ===================================================================================

// Мы можем сложить разные купюры в вектор длины '8', если напишем функцию, которая вычисляет индекс купюры в этом массиве по её номиналу
int GetIndex(Nominal n) {
    if (n == 10)
        return 0;
    if (n == 50)
        return 1;
    if (n == 100)
        return 2;
    if (n == 200)
        return 3;
    if (n == 500)
        return 4;
    if (n == 1000)
        return 5;
    if (n == 2000)
        return 6;
    if (n == 5000)
        return 7;
    return -1;
}

int main() {
    int nominal{};
    std::vector<int> cash(8);
    std::array<int, 8> nominal_names = {10, 50, 100, 200, 500, 1'000, 2'000, 5'000};

    std::ifstream input("input.txt");
    RedirectStandardInput redirection(input);

    while (std::cin >> nominal) {
        ++cash[GetIndex(nominal)];
    }

    for (size_t i = 0; i < cash.size(); ++i) {
        std::cout << nominal_names[i] << ": " << cash[i] << '\n';
    }

    return 0;
}
