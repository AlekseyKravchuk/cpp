#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std::literals;

/*
В этой задаче вам нужно присваивать номера автобусным маршрутам.
А именно, для каждого маршрута, заданного набором названий остановок, нужно либо выдать новый номер (первому маршруту — 1, второму — 2 и т. д.),
либо вернуть номер существующего маршрута, которому соответствует такой набор остановок.

Наборы остановок, полученные друг из друга перестановкой остановок, считаются различными (см. пример).

Указание:
В C++ ключом словаря может быть не только число или строка, но и другой контейнер, например, vector.

Формат ввода:
    - Сначала вводится количество запросов Q, затем Q описаний запросов.
    - Каждый запрос представляет собой положительное количество остановок N, за которым следуют разделённые пробелом N различных названий остановок соответствующего маршрута. Названия остановок состоят лишь из латинских букв и символов подчёркивания.

Формат вывода:
    - Выведите ответ на каждый запрос в отдельной строке.
    - Если маршрут с данным набором остановок уже существует, в ответ на соответствующий запрос выведите Already exists for i, где i — номер маршрута с таким набором остановок.
      В противном случае нужно выделить введённому набору остановок новый номер i и вывести его в формате New bus i.

Пример 1:
Ввод:
4
2 Marushkino Kokoshkino
1 Kokoshkino
2 Marushkino Kokoshkino
2 Kokoshkino Marushkino

Вывод:
New bus 1
New bus 2
Already exists for 1
New bus 3

Замечание:
Будьте внимательны при обновлении словаря: если одновременно в одном выражении вычислять размер словаря и добавлять в него новый элемент,
результат может отличаться от ожидаемого на единицу. Чтобы этого избежать, используйте временную переменную.

Например, вместо кода

map<string, int> m;
// ...
m["two"] = m.size();
// что нужно сделать раньше: вычислить size или добавить новый ключ?

стоит написать такой код:

map<string, int> m;
// ...
int new_value = m.size();
m["two"] = new_value;
// теперь всё однозначно: нужен размер до добавления элемента
*/

void ProcessOperations(int n) {
    std::map<std::vector<std::string>, int> stops2route;
    int num_stops{};
    size_t route_number{1};

    for (int i = 0; i < n; ++i) {
        std::cin >> num_stops;
        std::string stop_name{};
        std::vector<std::string> stops;
        stops.reserve(num_stops);

        for (int k = 0; k < num_stops; ++k) {
            std::cin >> stop_name;
            stops.push_back(stop_name);
        }
        
        if (stops2route.count(stops)) {
            std::cout << "Already exists for "s << stops2route[stops] << std::endl;
        } else {
            stops2route[stops] = route_number++;
            std::cout << "New bus "s << stops2route[stops] << std::endl;
        }
    }
}

int main() {
    // std::ifstream in("31_task_input.txt");      // configuring input from the file "31_task_input.txt"
    // std::streambuf* cinbuf = std::cin.rdbuf();  // save old buf
    // std::cin.rdbuf(in.rdbuf());                 // redirect std::cin to "31_task_input.txt"!

    int n{};
    std::cin >> n;
    ProcessOperations(n);

    // std::cin.rdbuf(cinbuf);  // reset to standard input again

    return 0;
}
