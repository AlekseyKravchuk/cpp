#include <iostream>
#include <sstream>  // std::istringstream
#include <stdexcept>
#include <string>
#include <vector>

#include "condition_parser.h"
#include "database.h"
#include "date.h"
#include "node.h"
#include "test_runner.h"

std::string ParseEvent(std::istream& iss) {
    // Реализуйте эту функцию
    std::string str;
    iss >> str;
    return str;
}

void TestAll();

int main() {
    TestAll();

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
}

void TestParseEvent() {
    {
        std::istringstream iss("event");
        AssertEqual(ParseEvent(iss), "event", "Parse event without leading spaces");
    }
    {
        std::stringstream iss("   sport event ");
        AssertEqual(ParseEvent(iss), "sport event ", "Parse event with leading spaces");
    }
    {
        std::istringstream iss("  first event  \n  second event");
        std::vector<std::string> events;
        events.push_back(ParseEvent(iss));
        events.push_back(ParseEvent(iss));
        AssertEqual(events, std::vector<std::string>{"first event  ", "second event"}, "Parse multiple events");
    }
}

void TestAll() {
    TestRunner tr;
    tr.RunTest(TestParseEvent, "TestParseEvent");
    tr.RunTest(TestParseCondition, "TestParseCondition");
}
