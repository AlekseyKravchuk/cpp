#include <iostream>
#include <stdexcept>
#include <string>

#include "condition_parser.h"
#include "database.h"
#include "date.h"
#include "input_redirection.h"
#include "tests.h"

using namespace std::literals;

// #define _GLIBCXX_DEBUG 1  // включить режим отладки

int main() {
     TestAll();

#ifdef _GLIBCXX_DEBUG
    // =========== Standard input redirection, debug mode ===========
    std::string path = "/home/kav/opt/clionProjects/yellow_belt_final/input.txt"s;
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("File \""s + path + "\" is not opened"s);
    }
    RedirectStandardInput redirection(input);
    // =================== End of input redirection ==================
#endif  //_GLIBCXX_DEBUG

    Database db;

    for (std::string line; std::getline(std::cin, line);) {
        std::istringstream iss(line);

        std::string command;
        iss >> command;
        if (command == "Add") {
            const auto date = ParseDate(iss);
            const auto event = ParseEvent(iss);
            db.Add(date, event);
        } else if (command == "Print") {
            db.Print(std::cout);
        } else if (command == "Del") {
            auto condition = ParseCondition(iss);

            auto predicate = [condition](const Date& date, const std::string& event) {
                return condition->Evaluate(date, event);
            };
            
            int count = db.RemoveIf(predicate);
            std::cout << "Removed " << count << " entries" << std::endl;
        } else if (command == "Find") {
            auto condition = ParseCondition(iss);

            auto predicate = [condition](const Date& date, const std::string& event) {
                return condition->Evaluate(date, event);
            };

            const auto entries = db.FindIf(predicate);
            for (const auto& entry : entries) {
                std::cout << entry << std::endl;
            }
            std::cout << "Found " << entries.size() << " entries" << std::endl;
        } else if (command == "Last") {
            try {
                std::cout << db.Last(ParseDate(iss)) << std::endl;
            } catch (std::invalid_argument&) {
                std::cout << "No entries" << std::endl;
            }
        } else if (command.empty()) {
            continue;
        } else {
            throw std::logic_error("Unknown command: " + command);
        }
    }

    return 0;
}  // по этой закрывающей скобке сработает деструктор ~RedirectStandardInput() и стандартный ввод будет восстановлен
