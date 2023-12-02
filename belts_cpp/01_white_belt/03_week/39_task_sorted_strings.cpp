#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std::literals;

/*
Задание по программированию «Отсортированные строки»

Реализуйте класс, поддерживающий набор строк в отсортированном порядке. Класс должен содержать два публичных метода:

class SortedStrings {
public:
  void AddString(const string& s) {
    // добавить строку s в набор
  }
  vector<string> GetSortedStrings() {
    // получить набор из всех добавленных строк в отсортированном порядке
  }
private:
  // приватные поля
};


Пример
Код

void PrintSortedStrings(SortedStrings& strings) {
  for (const string& s : strings.GetSortedStrings()) {
    cout << s << " ";
  }
  cout << endl;
}

int main() {
  SortedStrings strings;

  strings.AddString("first");
  strings.AddString("third");
  strings.AddString("second");
  PrintSortedStrings(strings);

  strings.AddString("second");
  PrintSortedStrings(strings);

  return 0;
}

Вывод

first second third
first second second third
f
*/

class SortedStrings {
   public:
    void AddString(const std::string& s) {
        _strings.push_back(s);
        std::sort(_strings.begin(), _strings.end());
    }

    std::vector<std::string> GetSortedStrings() {
        return _strings;
    }

   private:
    std::vector<std::string> _strings;
};

void PrintSortedStrings(SortedStrings& strings) {
    for (const std::string& s : strings.GetSortedStrings()) {
        std::cout << s << " ";
    }
    std::cout << std::endl;
}

int main() {
    SortedStrings strings;

    strings.AddString("first");
    strings.AddString("third");
    strings.AddString("second");
    PrintSortedStrings(strings);

    strings.AddString("second");
    PrintSortedStrings(strings);
    return 0;
}
