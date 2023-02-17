#include <algorithm>
#include <cmath>    // std::abs
#include <fstream>  //
#include <iostream>
#include <iterator>  // std::make_move_iterator
#include <map>
#include <string>
#include <vector>

/*
Задание по программированию «Ежемесячные дела»

У каждого из нас есть повторяющиеся ежемесячные дела, каждое из которых нужно выполнять в конкретный день каждого месяца:
оплата счетов за электричество, абонентская плата за связь и пр.
Вам нужно реализовать работу со списком таких дел на месяц, а именно, реализовать набор следующих операций:

ADD i s <=> Назначить дело с названием s на день i текущего месяца.

DUMP i  <=> Вывести все дела, запланированные на день i текущего месяца.

NEXT    <=> Перейти к списку дел на новый месяц.
            При выполнении данной команды вместо текущего (старого) списка дел на текущий месяц
            создаётся и становится активным (новый) список дел на следующий месяц:
            все дела со старого списка дел копируются в новый список.
            После выполнения данной команды новый список дел и следующий месяц становятся текущими, а работа со старым списком дел прекращается.
            При переходе к новому месяцу необходимо обратить внимание на разное количество дней в месяцах:
                если следующий месяц имеет больше дней, чем текущий, «дополнительные» дни необходимо оставить пустыми (не содержащими дел);
                если следующий месяц имеет меньше дней, чем текущий, меньше дней, дела со всех «лишних» дней необходимо переместить на последний день следующего месяца.

Замечания:

    - Историю списков дел хранить не требуется, работа ведется только с текущим списком дел текущего месяца.
    - Более того, при создании списка дел на следующий месяц, он «перетирает» предыдущий список.
    - Обратите внимание, что количество команд NEXT в общей последовательности команд при работе со списком дел может превышать 11.
    - Начальным текущим месяцем считается январь.
    - Количества дней в месяцах соответствуют Григорианскому календарю с той лишь разницей, что в феврале всегда 28 дней.

Формат ввода:
    Сначала число операций Q, затем описания операций.
    Названия дел s уникальны и состоят только из латинских букв, цифр и символов подчёркивания.
    Номера дней i являются целыми числами и нумеруются от 1 до размера текущего месяца.

Формат вывода:
    Для каждой операции типа DUMP в отдельной строке выведите количество дел в соответствующий день, а затем их названия, разделяя их пробелом. Порядок вывода дел в рамках каждой операции значения не имеет.

Пример:

Ввод:
12
ADD 5 Salary
ADD 31 Walk
ADD 30 WalkPreparations
NEXT
DUMP 5
DUMP 28
NEXT
DUMP 31
DUMP 30
DUMP 28
ADD 28 Payment
DUMP 28

Вывод:
1 Salary
2 WalkPreparations Walk
0
0
2 WalkPreparations Walk
3 WalkPreparations Walk Payment


Указание:
    Для дописывания всех элементов вектора v2 в конец вектора v1 удобно использовать метод insert:
    v1.insert(end(v1), begin(v2), end(v2));
 */

void PrintTasksPerDay(const std::vector<std::string>& tasks) {
    std::cout << tasks.size();

    for (const auto& task : tasks) {
        std::cout << ' ' << task;
    }

    std::cout << std::endl;
}

void ProcessOperations(int n) {
    enum class OPERATIONS { ADD,
                            DUMP,
                            NEXT
    };

    std::map<std::string, OPERATIONS> str2op = {
        {"ADD", OPERATIONS::ADD},
        {"DUMP", OPERATIONS::DUMP},
        {"NEXT", OPERATIONS::NEXT}};

    // массив с количеством дней в каждом месяце года
    int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int month_index{0};  // всегда начинаем с Января

    std::vector<std::vector<std::string>> v(days[month_index]);

    std::string op_as_str;
    int day{};
    std::string task_name{};

    for (int i = 0; i < n; ++i) {
        std::cin >> op_as_str;
        switch (str2op[op_as_str]) {
            case OPERATIONS::ADD: {
                std::cin >> day;
                std::cin >> task_name;
                --day;  // поскольку элементы вектора нумеруются с нуля
                v[day].push_back(task_name);
                break;
            }
            case OPERATIONS::DUMP: {
                std::cin >> day;
                --day;  // поскольку элементы вектора нумеруются с нуля
                PrintTasksPerDay(v[day]);
                break;
            }
            case OPERATIONS::NEXT: {
                int next_index = (month_index + 1) % 12;
                int delta = days[next_index] - days[month_index];

                // если в следующем месяце дней больше, чем в текущем,
                // «дополнительные» дни оставляем пустыми (не содержащими дел);

                // если в следующем месяце меньше дней, чем в текущем,
                // дела со всех «лишних» дней необходимо переместить на последний день следующего месяца.
                // например, Январь => Февраль (31 день => 28 дней): задачи с 31-го, 30-го и 29-го нужно переместить на 28-ой день
                if (delta < 0) {
                    // вычисляем индекс вектора строк, В КОТОРЫЙ будем перемещать дела (строки) из "лишних" дней
                    auto dst_index = v.size() - 1 - abs(delta);

                    // "i" - это индекс источника, откуда будем перемещать дела
                    for (auto i = dst_index + 1; i < v.size(); ++i) {
                        auto src_begin = std::make_move_iterator(v[i].begin());
                        auto src_end = std::make_move_iterator(v[i].end());

                        v[dst_index].reserve(v[dst_index].size() + v[i].size());
                        v[dst_index].insert(v[dst_index].end(), src_begin, src_end);
                    }
                }
                month_index = next_index;
                v.resize(v.size() + delta, std::vector<std::string>());

                break;
            }
            default:
                break;
        }
    }
}

int main() {
    // std::ifstream in("25_task_input.txt");  // configuring input from the file "25_task_input.txt"

    // if (in) {
    //     std::cout << "File opened." << std::endl;
    // } else {
    //     std::cout << "Opening file failed." << std::endl;
    // }

    // std::streambuf* cinbuf = std::cin.rdbuf();  // save old buf
    // std::cin.rdbuf(in.rdbuf());                 // redirect std::cin to "25_task_input.txt"!

    int n{};
    std::cin >> n;
    ProcessOperations(n);

    // std::cin.rdbuf(cinbuf);  // reset to standard input again

    return 0;
}
