#include "synonyms.h"
#include "operation_codes.h"
#include "input_redirection.h"
#include "tests.h"


int main() {
    // Запускаем unit-тесты
    // Функция "TestAll" должна идти самым первым вызовом в функции main
    TestAll();

    // ========= redirection of standard input to the file =========
    std::string path = "input.txt"s;
    std::ifstream input;
    RedirectStandardInput redirection(input, path);
    // ================== END of redirection ========================

    int num_queries;
    std::cin >> num_queries;  // считываем количество запросов, которое нам дано

    // объявляем словарь синонимов: каждому слову соответствует МНОЖЕСТВО слов-синонимов
    std::map<std::string, std::set<std::string>> synonyms;

    for (int i = 0; i < num_queries; ++i) {
        std::string operation_code;
        std::cin >> operation_code;

        switch (str2opcode[operation_code]) {
            case OPCODE::ADD: {
                std::string first_word, second_word;
                std::cin >> first_word >> second_word;
                AddSynonyms(synonyms, first_word, second_word);
                break;
            }
            case OPCODE::COUNT: {
                std::string word;
                std::cin >> word;
                std::cout << GetSynonymsCount(synonyms, word) << std::endl;
                break;
            }
            case OPCODE::CHECK: {
                std::string first_word, second_word;
                std::cin >> first_word >> second_word;
                std::cout << ((AreSynonyms(synonyms, first_word, second_word)) ? "YES"s : "NO"s) << std::endl;
                break;
            }
            default: {
                throw std::runtime_error("Invalid operation code: " + operation_code);
            }
        }
    }

    return 0;
}  // по закрывающей скобке будет вызван деструктор класса "RedirectStandardInput", в котором будет восстановлен стандартный ввод
