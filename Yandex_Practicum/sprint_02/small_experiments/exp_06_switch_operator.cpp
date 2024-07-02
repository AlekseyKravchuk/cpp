#include <iostream>
#include <set>

using namespace std;

enum class RequestType {
    ADD,
    REMOVE,
    NEGATE
};

/*
В нашем случае нумерация перечислителей в наборе не имела значения.
Но когда числовые значения элементов enum важны, их можно указать явно:
*/
// enum class RequestType {
//     ADD = 9,
//     REMOVE = 8,
//     NEGATE = 7
// };

void ProcessRequest(
    set<int>& numbers,
    RequestType request_type,
    int request_data) {
    switch (request_type) {
        case RequestType::ADD:
            numbers.insert(request_data);
            break;
        case RequestType::REMOVE:
            numbers.erase(request_data);
            break;
        case RequestType::NEGATE: {  // фигурные скобки обязательны
            bool contains = numbers.count(request_data) == 1;
            if (contains) {
                numbers.erase(request_data);
                numbers.insert(-request_data);
            }
            break;
        }
        default:
            cout << "Unknown request" << endl;
    }
}

int main() {
    /*  Если типы запросов поступают на вход программы в виде чисел 9, 8 и 7.
    Тогда получить объект типа RequestType по его числовому коду можно будет простым оператором static_cast: */

    int request_code;
    cin >> request_code;

    auto request_type = static_cast<RequestType>(request_code);

    // Если ввести request_code = 7,
    // в переменной request_type окажется RequestType::NEGATE
    // Если преобразуемому числовому коду не будет соответствовать ни один элемент перечислимого типа, ошибки компиляции не произойдёт.
    // Но дальнейшее поведение программы грозит стать непредсказуемым.
    return 0;
}
