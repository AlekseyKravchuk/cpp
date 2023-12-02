#include <algorithm>
#include <iostream>
#include <iterator>  // std::prev
#include <map>
#include <string>
#include <vector>

using namespace std::literals;

/*


Задание по программированию «Структура LectureTitle»

Дана структура LectureTitle:

struct LectureTitle {
  string specialization;
  string course;
  string week;
};


Допишите конструктор и структуры Specialization, Course, Week так, чтобы объект LectureTitle можно было создать с помощью кода

LectureTitle title(
    Specialization("C++"),
    Course("White belt"),
    Week("4th")
);

 
но нельзя было с помощью следующих фрагментов кода:

LectureTitle title("C++", "White belt", "4th");

LectureTitle title(string("C++"), string("White belt"), string("4th"));

LectureTitle title = {"C++", "White belt", "4th"};

LectureTitle title = {{"C++"}, {"White belt"}, {"4th"}};

LectureTitle title(
    Course("White belt"),
    Specialization("C++"),
    Week("4th")
);

LectureTitle title(
    Specialization("C++"),
    Week("4th"),
    Course("White belt")
);
*/
struct Specialization {
    std::string value{};
    explicit Specialization(const std::string& new_value) : value(new_value) {}
};

struct Course {
    std::string value{};
    explicit Course(const std::string& new_value) : value(new_value) {}
};

struct Week {
    std::string value{};
    explicit Week(const std::string& new_value) : value(new_value) {}
};

struct LectureTitle {
    std::string specialization;
    std::string course;
    std::string week;
    explicit LectureTitle(const Specialization& new_spec, const Course& new_course, const Week& new_week)
        : specialization(new_spec.value), course(new_course.value), week(new_week.value) {}
};

int main() {
    LectureTitle title(
        Specialization("C++"),
        Course("White belt"),
        Week("4th"));

    // LectureTitle title("C++", "White belt", "4th");

    // LectureTitle title(std::string("C++"), std::string("White belt"), std::string("4th"));

    // LectureTitle title = {"C++", "White belt", "4th"};

    // LectureTitle title = {{"C++"}, {"White belt"}, {"4th"}};

    return 0;
}