#include <algorithm>
#include <iostream>
#include <map>
#include <string>

using namespace std;

int main() {
    const int valueToSearch = 1;

    map<int, string> myMap;
    myMap[0] = "первый"s;
    myMap[1] = "второй"s;
    myMap[2] = "третий"s;
    // результаты этих двух поисков будут одинаковы,

    // но первый будет сделан более эффективно
    auto itMoreEfficient = myMap.find(valueToSearch);

    // НИЖЕ следующий код почему-то вообще НЕ КОМПИЛИРУЕТСЯ !!
    // error: no match for ‘operator==’ (operand types are ‘std::pair<const int, std::__cxx11::basic_string<char> >’ and ‘const int’) 

    // ни в таком варианте:
    auto it = std::find(myMap.begin(), myMap.end(), valueToSearch);

    // ни в таком:
    // std::map<int, string>::iterator it = std::find(myMap.begin(), myMap.end(), 1);
}