#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std::literals;
using namespace std::chrono;

struct Person {
    std::string name;
    std::string surname;
    int age;
};

std::vector<Person> GetMoscowPopulation() {
    return std::vector<Person>{
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
        {"Ivan"s, "Ivanov"s, 25},
    };
}

void PrintPopulationSize(/* const */ std::vector<Person>& p) {
    std::cout << "There are "s << p.size() << " people in Moscow."s << std::endl;
}

int main() {
    auto start = steady_clock::now();
    std::vector<Person> people = GetMoscowPopulation();
    auto finish = steady_clock::now();
    std::cout << "GetMoscowPopulation(): "s << duration_cast<microseconds>(finish - start).count() << " microseconds"s << std::endl;

    start = steady_clock::now();

    // TODO:
    PrintPopulationSize(GetMoscowPopulation());
    finish = steady_clock::now();
    std::cout << "PrintPopulationSize(people): "s << duration_cast<microseconds>(finish - start).count() << " microseconds"s << std::endl;

    return 0;
}
