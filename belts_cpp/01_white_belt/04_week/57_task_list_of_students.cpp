#include <fstream>
#include <iomanip>
#include <iostream>  // std::streambuf
#include <istream>   // std::ws - Discards leading whitespace from an input stream
#include <sstream>
#include <string>
#include <vector>

using namespace std::literals;

// #define _GLIBCXX_DEBUG 1  // включить режим отладки

/*
Задание по программированию «Список студентов»

Определите структуру «Студент» со следующими полями: имя, фамилия, день, месяц и год рождения. Создайте вектор из таких структур, заполните его из входных данных и затем по запросам выведите нужные поля.

Формат ввода:
- Первая строка содержит одно целое число N от 0 до 10000 — число студентов.
- Далее идут N строк, каждая из которых содержит две строки длиной от 1 до 15 символов — имя и фамилию очередного студента,
  и три целых числа от 0 до 1000000000 — день, месяц и год рождения.
- Следующая строка содержит одно целое число M от 0 до 10000 — число запросов.
- Следующие M строк содержат строку длиной от 1 до 15 символов — запрос, и целое число от 0 до 1000000000 — номер студента (нумерация начинается с 1).

Формат вывода:
- Для запроса вида name K, где K от 1 до N, выведите через пробел имя и фамилию K-го студента.
- Для запроса вида date K, где K от 1 до N, выведите через точку день, месяц и год рождения K-го студента.
- Для остальных запросов выведите bad request.

Пример ввода:
3
Ivan Ivanov 1 1 1901
Petr Petrox 2 2 1902
Alexander Sidorov 3 3 1903
3
name 1
date 3
mark 5

Пример вывода:
Ivan Ivanov
3.3.1903
bad request
*/

struct Student {
    std::string name{};
    std::string surname{};
    int day{};
    int month{};
    int year{};
};

bool IsQueryTypeValid(const std::string& query_type) {
    return query_type == "name"s || query_type == "date"s;
}

bool IsQueryArgInRange(int arg, size_t sz) {
    return (arg >= 0) && (arg < sz) && (arg < 10000);
}

int main() {
#ifdef _GLIBCXX_DEBUG
    // === debug mode is enabled ===
    std::string path = "input.txt"s;
    std::ifstream in(path);

    // перенаправляем поток ввода std::cin на файл "input.txt"
    // std::ios::rdbuf имеет 2 перегруженные формы:
    // 1) getter: streambuf* rdbuf() const; // возвращает указатель на объект "stream buffer" ("std::streambuf"),
    //                                      // который был ассоциирован с потоком до вызова "std::ios::rdbuf()"
    std::streambuf* cinbuf_bkp = std::cin.rdbuf();  // сохраняем указатель на "stream buffer", который ассоциирован с объектом std::cin

    // 2) setter: streambuf* rdbuf (streambuf* sb);  // ассоциирует буфер потока "std::streambuf", на который указывает указатель"sb", с объектом-потоком,
    //                                                  посредством которого вызывается setter "rdbuf(streambuf* sb)"
    std::cin.rdbuf(in.rdbuf());  // redirect std::cin to "input.txt"!
#endif                           // _GLIBCXX_DEBUG

    std::vector<Student> students;
    int n{};
    std::cin >> n;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (int i = 0; i < n; ++i) {
        std::string line{};
        std::getline(std::cin, line);
        std::istringstream iss(line);

        Student student;
        iss >> student.name >> student.surname >> student.day >> student.month >> student.year;
        students.push_back(student);
    }

    int num_queries{};
    std::cin >> num_queries;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (int j = 0; j < num_queries; ++j) {
        std::string line{};
        std::getline(std::cin, line);
        std::istringstream iss(line);

        std::string query_type{};
        int id;
        iss >> query_type >> id >> std::ws;
        --id;  // поскольку нумерация "student id" начинается с нуля

        std::string trash{};
        iss >> trash;
        if (!trash.empty()) {
            std::cout << "bad request"s << std::endl;
            continue;
        }

        if (IsQueryTypeValid(query_type) && IsQueryArgInRange(id, students.size())) {
            if (query_type == "name"s) {
                std::cout << students[id].name << ' ' << students[id].surname << std::endl;
            } else if (query_type == "date"s) {
                std::cout << students[id].day << '.'
                          << students[id].month << '.'
                          << students[id].year << std::endl;
            } else {
                std::cout << "bad request"s << std::endl;
            }
        } else {
            std::cout << "bad request"s << std::endl;
        }
    }

#ifdef _GLIBCXX_DEBUG
    std::cin.rdbuf(cinbuf_bkp);  // восстанавливаем standard input
#endif

    return 0;
}
