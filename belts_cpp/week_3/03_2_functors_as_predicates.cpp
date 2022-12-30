// пусть имеется некоторая таблица с результатами вступительных экзаменов абитуриентов
// требуется вывести зачисленных в ВУЗ абитуриентов, проходной балл (score) равен 180 баллам

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

template <typename Iterator>
void PrintRange(Iterator beginIt, Iterator endIt) {
    using namespace std::literals;

    for (auto it = beginIt; it != endIt; ++it) {
        if (std::next(it) != endIt) {
            std::cout << *it << " "s;
        } else {
            std::cout << *it << std::endl;
        }
    }
}

template <typename Collection>
void PrintCollection(const Collection& collection) {
    using namespace std::literals;

    bool isFirst = true;
    for (const auto& elm : collection) {
        if (isFirst) {
            std::cout << elm;
            isFirst = false;
        } else {
            std::cout << " "s << elm;
        }
    }
    std::cout << std::endl;
}

struct Person {
    std::string _name;
    double _score;
};

// для того, чтобы можно было пользоваться "PrintCollection", нужно переопределить оператор "<<"
std::ostream& operator<<(std::ostream& os, const Person& p) {
    os << "{"s << p._name << ", "s << p._score << "}"s;
    return os;
}

int main() {
    // задаем проходной балл
    int threshold = 180;

    std::vector<Person> applicants = {
        {"Вася"s, 181},
        {"Петя"s, 30},
        {"Маша"s, 179},
        {"Даша"s, 200},
        {"Катя"s, 198},
        {"Андрей"s, 181},
        {"Сергей"s, 50},
        {"Иван"s, 150},
    };

    // создаем placeholder для отсортированного массива
    std::vector<Person> admittedApplicants;
    std::copy_if(applicants.begin(), applicants.end(),
                std::back_inserter(admittedApplicants),
                [threshold](const Person& p){
                    return p._score >= threshold;
                });

    PrintCollection(admittedApplicants);

    return 0;
}
