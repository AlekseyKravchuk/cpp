#include <iostream>
#include <string>

using namespace std::literals;

struct Lecture {
    std::string title{};
    int duration{};
};

void PrintLecture(const Lecture& lecture) {
    std::cout << "Title: "s << lecture.title
              << ", duration: "s << lecture.duration
              << std::endl;
}

int main() {
    // Lecture lecture = {"OOP"s, 5400};
    // Lecture lecture;
    Lecture lecture = {"OOP"s};

    PrintLecture(lecture);

    return 0;
}
