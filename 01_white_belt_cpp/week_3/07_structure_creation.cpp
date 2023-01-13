#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

struct Lecture {
    std::string title {};
    int duration {};
    std::string author {};
};

void PrintLecture(const Lecture& lecture) {
    std::cout << "Title: "s << lecture.title
              << ", duration: "s << lecture.duration
              << ", author: "s << lecture.author
              << std::endl;
}

void PrintCourse(const std::vector<Lecture>& lectures) {
    for (const Lecture& lecture : lectures) {
        PrintLecture(lecture);
    }
}

int main() {
    std::vector<Lecture> lectures = {
        {"OOP", 5400, "Anton"},
        {"STL", 4800, "Mike"},
        {"Iterators", 5000, "Alex"}
    };

    PrintCourse(lectures);
    
    return 0;
}
