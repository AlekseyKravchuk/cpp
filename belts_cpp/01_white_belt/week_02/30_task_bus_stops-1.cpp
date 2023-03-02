#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <utility>  // std::move
#include <vector>

using namespace std::literals;

/*
Задание по программированию «Автобусные остановки — 1»

Реализуйте систему хранения автобусных маршрутов. Вам нужно обрабатывать следующие запросы:

    NEW_BUS bus stop_count stop1 stop2 ... — добавить маршрут автобуса с названием bus и stop_countостановками с названиями stop1, stop2, ...

    BUSES_FOR_STOP stop — вывести названия всех маршрутов автобуса, проходящих через остановку stop.

    STOPS_FOR_BUS bus — вывести названия всех остановок маршрута bus со списком автобусов, на которые можно пересесть на каждой из остановок.

    ALL_BUSES — вывести список всех маршрутов с остановками.

Формат ввода

В первой строке ввода содержится количество запросов Q, затем в Q строках следуют описания запросов.

Гарантируется, что все названия маршрутов и остановок состоят лишь из латинских букв, цифр и знаков подчёркивания.

Для каждого запроса NEW_BUS bus stop_count stop1 stop2 ... гарантируется, что маршрут bus отсутствует, количество остановок больше 0, а после числа stop_count следует именно такое количество названий остановок, причём все названия в каждом списке различны.
Формат вывода

Для каждого запроса, кроме NEW_BUS, выведите соответствующий ответ на него:

    На запрос BUSES_FOR_STOP stop выведите через пробел список автобусов, проезжающих через эту остановку, в том порядке, в котором они создавались командами NEW_BUS. Если остановка stop не существует, выведите No stop.

    На запрос STOPS_FOR_BUS bus выведите описания остановок маршрута bus в отдельных строках в том порядке, в котором они были заданы в соответствующей команде NEW_BUS. Описание каждой остановки stop должно иметь вид Stop stop: bus1 bus2 ..., где bus1 bus2 ... — список автобусов, проезжающих через остановку stop, в порядке, в котором они создавались командами NEW_BUS, за исключением исходного маршрута bus. Если через остановку stop не проезжает ни один автобус, кроме bus, вместо списка автобусов для неё выведите no interchange. Если маршрут bus не существует, выведите No bus.

    На запрос ALL_BUSES выведите описания всех автобусов в алфавитном порядке. Описание каждого маршрута bus должно иметь вид Bus bus: stop1 stop2 ..., где stop1 stop2 ... — список остановок автобуса bus в порядке, в котором они были заданы в соответствующей команде NEW_BUS. Если автобусы отсутствуют, выведите No buses.

Предупреждение:
Условие задачи выше содержит много важных деталей. Если вы не уверены в том, что не упустили ни одной, перечитайте условие ещё раз.

Пример 1

Ввод:
10
ALL_BUSES
BUSES_FOR_STOP Marushkino
STOPS_FOR_BUS 32K
NEW_BUS 32 3 Tolstopaltsevo Marushkino Vnukovo
NEW_BUS 32K 6 Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo
BUSES_FOR_STOP Vnukovo
NEW_BUS 950 6 Kokoshkino Marushkino Vnukovo Peredelkino Solntsevo Troparyovo
NEW_BUS 272 4 Vnukovo Moskovsky Rumyantsevo Troparyovo
STOPS_FOR_BUS 272
ALL_BUSES

Вывод:
No buses
No stop
No bus
32 32K
Stop Vnukovo: 32 32K 950
Stop Moskovsky: no interchange
Stop Rumyantsevo: no interchange
Stop Troparyovo: 950
Bus 272: Vnukovo Moskovsky Rumyantsevo Troparyovo
Bus 32: Tolstopaltsevo Marushkino Vnukovo
Bus 32K: Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo
Bus 950: Kokoshkino Marushkino Vnukovo Peredelkino Solntsevo Troparyovo

*/
template <typename T>
void PrintVector(std::vector<T> v) {
    bool isFirst = true;
    for (const auto& elm : v) {
        if (isFirst) {
            std::cout << elm;
            isFirst = false;
        } else {
            std::cout << ' ' << elm;
        }
    }
    std::cout << std::endl;
}

template <typename T>
void PrintVectorExcluding(std::vector<T> v, const T& exclude) {
    bool isFirst = true;
    for (const auto& elm : v) {
        if (isFirst) {
            if (elm != exclude) {
                std::cout << elm;
            }
            isFirst = false;
        } else {
            if (elm != exclude) {
                std::cout << ' ' << elm;
            }
        }
    }
    std::cout << std::endl;
}

void ProcessOperations(int n) {
    using std::cin;
    using std::cout;
    using std::endl;
    using std::map;
    using std::string;
    using std::vector;

    enum class OPERATIONS {
        NEW_BUS,
        BUSES_FOR_STOP,
        STOPS_FOR_BUS,
        ALL_BUSES
    };

    map<string, OPERATIONS> str2op = {
        {"NEW_BUS", OPERATIONS::NEW_BUS},
        {"BUSES_FOR_STOP", OPERATIONS::BUSES_FOR_STOP},
        {"STOPS_FOR_BUS", OPERATIONS::STOPS_FOR_BUS},
        {"ALL_BUSES", OPERATIONS::ALL_BUSES}};

    // название_маршрута : {список_названий_остановок}
    map<string, vector<string>> bus2stops;

    // название_автобусной_остановки : {список_маршрутов_проходящих_через_нее}
    map<string, vector<string>> stop2buses;

    string op_as_str{};

    for (int i = 0; i < n; ++i) {
        cin >> op_as_str;

        switch (str2op[op_as_str]) {
            case OPERATIONS::NEW_BUS: {
                int stop_count{};
                string bus{};  // маршрут автобуса с названием "bus"
                string stop;   // название автобусной остановки
                cin >> bus >> stop_count;

                for (int j = 0; j < stop_count; ++j) {
                    cin >> stop;
                    bus2stops[bus].push_back(stop);
                    stop2buses[stop].push_back(bus);
                }

                break;
            }
            case OPERATIONS::BUSES_FOR_STOP: {
                // название автобусной остановки, для которой нужно вывести названия всех маршрутов автобусов, останавливающихся на этой остановке
                string stop{};

                cin >> stop;

                if (stop2buses.count(stop)) {
                    PrintVector(stop2buses[stop]);
                } else {
                    cout << "No stop"s << endl;
                }

                break;
            }
            case OPERATIONS::STOPS_FOR_BUS: {
                string bus;
                cin >> bus;
                if (bus2stops.count(bus)) {
                    for (const auto& stop : bus2stops[bus]) {
                        cout << "Stop "s << stop << ": "s;
                        if (stop2buses.count(stop)) {
                            if (stop2buses[stop].size() == 1 && stop2buses[stop][0] == bus) {
                                cout << "no interchange"s << endl;
                            } else {
                                PrintVectorExcluding(stop2buses[stop], bus);
                            }
                        } else {
                            cout << "no interchange"s << endl;
                        }
                    }
                } else {
                    cout << "No bus"s << endl;
                }

                break;
            }
            case OPERATIONS::ALL_BUSES: {
                if (bus2stops.size()) {
                    for (const auto& [bus, _] : bus2stops) {
                        cout << "Bus "s << bus << ": "s;
                        PrintVector(bus2stops[bus]);
                    }
                } else {
                    cout << "No buses"s << endl;
                }
            }
            default:
                break;
        }
    }
}

int main() {
    std::ifstream in("30_task_input.txt");      // configuring input from the file "30_task_input.txt"
    std::streambuf* cinbuf = std::cin.rdbuf();  // save old buf
    std::cin.rdbuf(in.rdbuf());                 // redirect std::cin to "30_task_input.txt"!

    int n{};
    std::cin >> n;
    ProcessOperations(n);

    std::cin.rdbuf(cinbuf);  // reset to standard input again

    return 0;
}
