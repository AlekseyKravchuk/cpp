#include <algorithm>
#include <iostream>
#include <string>

using namespace std::literals;

/*
Задание по программированию «Строка»

Реализуйте класс ReversibleString, хранящий строку и поддерживающий методы Reverse для переворота строки и ToString для получения строки. 
Пример
Код

int main() {
  ReversibleString s("live");
  s.Reverse();
  cout << s.ToString() << endl;
  
  s.Reverse();
  const ReversibleString& s_ref = s;
  string tmp = s_ref.ToString();
  cout << tmp << endl;
  
  ReversibleString empty;
  cout << '"' << empty.ToString() << '"' << endl;
  
  return 0;
}

 
Вывод

evil
live
""
*/

class ReversibleString {
   private:
    std::string _str{};

   public:
    ReversibleString() = default;
    ReversibleString(const std::string& str) : _str(str) {}

    void Reverse() {
        std::reverse(_str.begin(), _str.end());
    }

    std::string ToString() const {
        return _str;
    }
};

int main() {
    ReversibleString s("live"s);
    s.Reverse();
    std::cout << s.ToString() << std::endl;

    s.Reverse();
    const ReversibleString& s_ref = s;
    std::string tmp = s_ref.ToString();
    std::cout << tmp << std::endl;

    ReversibleString empty;
    std::cout << '"' << empty.ToString() << '"' << std::endl;

    return 0;
}
