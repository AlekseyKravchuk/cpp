#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>  // std::istringstream
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>  // std::pair
#include <vector>

#include "bus_manager.h"
#include "input_redirection.h"
#include "query.h"
#include "responses.h"
#include "test_runner.h"
#include "tests.h"

using namespace std;

#define _GLIBCXX_DEBUG 1  // включить режим отладки

/*
===================== Декомпозиция программы — 2 =====================
Декомпозиция программы — 2

В задаче «Декомпозиция программы» мы разбили монолитный код на набор функций и классов.
Теперь мы сделаем ещё один шаг и разделим нашу программу на несколько файлов.
В этой задаче вам нужно создать проект, состоящий из следующих файлов:

1.query.h, в него кладём:

    enum class QueryType
    struct Query
    объявление istream& operator >> (istream& is, Query& q)

2. query.cpp, в него кладём

    определение istream& operator >> (istream& is, Query& q);

3. responses.h:

    struct BusesForStopResponse
    ostream& operator << (ostream& os, const BusesForStopResponse& r)
    struct StopsForBusResponse
    ostream& operator << (ostream& os, const StopsForBusResponse& r)
    struct AllBusesResponse
    ostream& operator << (ostream& os, const AllBusesResponse& r)

4. responses.cpp: определения всего, что объявлено в responses.h

5. bus_manager.h: объявление класса BusManager

6. bus_manager.cpp: определения методов класса BusManager

7. main.cpp: функция main

===================== Декомпозиция программы =====================
Введение

В курсе «Белый пояс по С++» у нас была задача «Автобусные остановки — 1».
В этой задаче вам будет дано её правильное решение, целиком содержащееся внутри функции main.
Вам надо будет выполнить декомпозицию этого решения на заранее заданные блоки так,
чтобы получившаяся программа так же корректно решала задачу.
Условие задачи «Автобусные остановки — 1» приведено ниже.

Исходные файлы:
    - Авторское решение этой задачи содержится в файле "correct.cpp" ("21_task_stepik_solution.cpp")
    - Кроме того, вам дан файл "starter.cpp", который содержит заготовки классов и функций. Не меняя функцию main,
      вам надо реализовать эти классы и функции так, чтобы получившаяся программа решала задачу «Автобусные остановки — 1».

Как будет тестироваться ваша программа:

Автоматическая проверяющая система заменит в вашей программе функцию main на ту, которая дана вам в файле starter.cpp, скомпилирует получившийся файл и прогонит на тестах для задачи «Автобусные остановки — 1».
Условие задачи «Автобусные остановки — 1»

Реализуйте систему хранения автобусных маршрутов. Вам нужно обрабатывать следующие запросы:

    NEW_BUS bus stop_count stop1 stop2 ... — добавить маршрут автобуса с названием bus и stop_count остановками с названиями stop1, stop2, ...

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
Пример

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

int main() {
    TestAll();

#ifdef _GLIBCXX_DEBUG
    // =========== Standard input redirection, debug mode ===========
    std::string path = "input.txt"s;
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("File \""s + path + "\" is not opened"s);
    }
    RedirectStandardInput redirection(input);
    // =================== End of input redirection ==================
#endif  //_GLIBCXX_DEBUG

    int query_count;
    Query q;

    std::cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        std::cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                std::cout << bm.GetBusesForStop(q.stop) << std::endl;
                break;
            case QueryType::StopsForBus:
                std::cout << bm.GetStopsForBus(q.bus) << std::endl;
                break;
            case QueryType::AllBuses:
                std::cout << bm.GetAllBuses() << std::endl;
                break;
        }
    }

    return 0;
}  // по этой закрывающей скобке сработает деструктор ~RedirectStandardInput() и стандартный ввод будет восстановлен
