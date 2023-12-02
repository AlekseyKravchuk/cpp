#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

/*
Разбиение на слова

Напишите функцию SplitIntoWords, разбивающую строку на слова по пробелам

vector<string> SplitIntoWords(const string& s);

Гарантируется, что:
    - строка непуста;
    - строка состоит лишь из латинских букв и пробелов;
    - первый и последний символы строки не являются пробелами;
    - строка не содержит двух пробелов подряд.

Подсказка

Рекомендуется следующий способ решения задачи:

    искать очередной пробел с помощью алгоритма find;

    создавать очередное слово с помощью конструктора строки по двум итераторам.

Пример кода

int main() {
  string s = "C Cpp Java Python";

  vector<string> words = SplitIntoWords(s);
  cout << words.size() << " ";
  for (auto it = begin(words); it != end(words); ++it) {
    if (it != begin(words)) {
      cout << "/";
    }
    cout << *it;
  }
  cout << endl;
  
  return 0;
}

Вывод:
4 С/Cpp/Java/Python
*/

vector<string> SplitIntoWords(const string& s) {
    if (s.empty()) {
        return {};
    }

    vector<string> v;
    auto it_first = s.begin();
    auto it_last = std::find(s.begin(), s.end(), ' ');

    while (it_last != s.end()) {
        v.emplace_back(it_first, it_last);
        it_first = it_last;

        while ((isspace(*it_first))) {
            ++it_first;
        }
        it_last = std::find(it_first, s.end(), ' ');
    }
    v.emplace_back(it_first, it_last);
    return v;
}

int main() {
    string s = "C Cpp Java Python";
    // string s = "one"s;
    // string s = "one two"s;

    vector<string> words = SplitIntoWords(s);
    cout << words.size() << " ";
    for (auto it = begin(words); it != end(words); ++it) {
        if (it != begin(words)) {
            cout << "/";
        }
        cout << *it;
    }
    cout << endl;

    return 0;
}