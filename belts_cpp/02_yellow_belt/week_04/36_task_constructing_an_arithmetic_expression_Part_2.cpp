#include <algorithm>
#include <cstdint>  // fixed width integer types uint8_t
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>

using namespace std;

// #define _GLIBCXX_DEBUG 1  // включить режим отладки

#ifdef _GLIBCXX_DEBUG
class RedirectStandardInput {
   public:
    RedirectStandardInput(std::ifstream& input) {
        // сохраняем указатель на "streambuf"
        _cinbuf_bkp = std::cin.rdbuf();

        // перенаправляем поток ввода std::cin на файл
        std::cin.rdbuf(input.rdbuf());
    }

    ~RedirectStandardInput() {
        std::cin.rdbuf(_cinbuf_bkp);  // восстанавливаем standard input
    }

   private:
    std::streambuf* _cinbuf_bkp{nullptr};
};
#endif  //_GLIBCXX_DEBUG

struct Operation {
    char type{};
    int number{};
};

int main() {
#ifdef _GLIBCXX_DEBUG
    // =========== Standard input redirection, debug mode ===========
    std::string path = "35_input.txt"s;
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("File \""s + path + "\" is not opened"s);
    }
    RedirectStandardInput redirection(input);
    // =================== End of input redirection ==================
#endif  //_GLIBCXX_DEBUG

    int initial_number;
    std::cin >> initial_number;

    int number_of_operations;
    const map<char, uint8_t> priorities = {
        {'+', 0},
        {'-', 0},
        {'*', 1},
        {'/', 1},
    };
    uint8_t previous_priority = 1;

    std::cin >> number_of_operations;
    vector<Operation> operations(number_of_operations);
    for (int i = 0; i < number_of_operations; ++i) {
        std::cin >> operations[i].type;
        std::cin >> operations[i].number;
    }

    deque<string> expressions;
    expressions.push_back(to_string(initial_number));
    for (const auto& operation : operations) {
        auto current_priority = priorities.at(operation.type);
        if (previous_priority < current_priority) {
            expressions.push_front("(");
            expressions.push_back(")");
        }
        previous_priority = priorities.at(operation.type);
        expressions.push_back(" ");
        expressions.push_back(string(1, operation.type));
        expressions.push_back(" ");
        expressions.push_back(to_string(operation.number));
    }

    for (const string& s : expressions) {
        cout << s;
    }

    std::cout << std::endl;


    return 0;
}
