#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    // int* intPtr = new int(18);
    // std::cout << intPtr << std::endl;  // вывели адрес, по которому хранится переменная типа int со значением 18
    // std::cout << *intPtr << std::endl;

    // выделим в куче память для хранения одного объекта типа "string"
    string* strPtr = new string;

    // для того, чтобы обратиться к этому объекту типа "string", разыменовываем указатель strPtr
    *strPtr = "Hello"s;

    // проверяем, что хранится по адресу strPtr:
    std::cout << *strPtr << std::endl;

    *strPtr += ", Alex!";

    // проверяем, что хранится по адресу strPtr:
    std::cout << *strPtr << std::endl;

    // хотим узнать размер нашей строки
    std::cout << (*strPtr).size() << std::endl;

    // есть более удобная форма записи
    std::cout << strPtr->size() << std::endl;

    // оператор *, примененный к указателю возвращает ссылку на объект в куче
    string& strRef = *strPtr;
    std::cout << strRef << std::endl;

    strRef += "And Mike!"s;
    std::cout << strRef << std::endl;

    // проверяем содержимое объекта на который указывает указатель "strPtr"
    std::cout << *strPtr << std::endl;

    return 0;
}
