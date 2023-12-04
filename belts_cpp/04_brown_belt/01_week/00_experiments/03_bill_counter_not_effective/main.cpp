#include <array>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

// Задача: написать счетчик купюр - хотим уметь считать сколько купюр каждого из номиналов у нас есть в наличии

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

int main() {
    int nominal{};

    // индексом в этом векторе будут номиналы купюр: {10, 50, 100, 200, 500, 1'000, 2'000, 5'000}
    // вынуждены завести вектор размера 5'001, т.к. индексация начинается с '0'
    std::vector<int> cash(5'001);

    std::ifstream input("input.txt");
    RedirectStandardInput redirection(input);

    while (std::cin >> nominal) {
        ++cash[nominal];
    }

    for (size_t i = 0; i < cash.size(); ++i) {
        if (cash[i]) {
            std::cout << i << ": " << cash[i] << '\n';
        }
    }

    return 0;
}
