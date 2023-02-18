#include <iostream>
#include <map>
#include <ostream>
#include <utility>
#include <vector>

using namespace std::literals;

/*
Задание по программированию «Справочник столиц»

Реализуйте справочник столиц стран.

На вход программе поступают следующие запросы:

    - CHANGE_CAPITAL country new_capital — изменение столицы страны country на "new_capital",
      либо добавление такой страны с такой столицей, если раньше её не было.

    - RENAME old_country_name new_country_name — переименование страны из old_country_name в new_country_name.

    - ABOUT country — вывод столицы страны "country".

    - DUMP — вывод столиц всех стран.

Формат ввода:
    - В первой строке содержится количество запросов Q, в следующих Q строках — описания запросов.
    - Все названия стран и столиц состоят лишь из латинских букв, цифр и символов подчёркивания.

Формат вывода:
Выведите результат обработки каждого запроса:
В ответ на запрос CHANGE_CAPITAL country "new_capital" выведите

    Introduce new country country with capital "new_capital", если страны country раньше не существовало;

    Country country hasn't changed its capital, если страна country до текущего момента имела столицу "new_capital";

    Country country has changed its capital from old_capital to "new_capital", если страна country до текущего момента имела столицу old_capital, название которой не совпадает с названием new_capital.

В ответ на запрос RENAME old_country_name new_country_name выведите

    Incorrect rename, skip, если новое название страны совпадает со старым, страна old_country_name не существует или страна new_country_name уже существует;

    Country old_country_name with capital capital has been renamed to new_country_name, если запрос корректен и страна имеет столицу capital.

В ответ на запрос ABOUT country выведите

    Country country doesn't exist, если страны с названием country не существует;

    Country country has capital capital, если страна country существует и имеет столицу capital.

В ответ на запрос DUMP выведите

    There are no countries in the world, если пока не было добавлено ни одной страны;

    последовательность пар вида country/capital, описывающую столицы всех стран, если в мире уже есть хотя бы одна страна. При выводе последовательности пары указанного вида необходимо упорядочить по названию страны и разделять между собой пробелом.

Пример 1

Ввод

6
CHANGE_CAPITAL RussianEmpire Petrograd
RENAME RussianEmpire RussianRepublic
ABOUT RussianRepublic
RENAME RussianRepublic USSR
CHANGE_CAPITAL USSR Moscow
DUMP

Вывод

Introduce new country RussianEmpire with capital Petrograd
Country RussianEmpire with capital Petrograd has been renamed to RussianRepublic
Country RussianRepublic has capital Petrograd
Country RussianRepublic with capital Petrograd has been renamed to USSR
Country USSR has changed its capital from Petrograd to Moscow
USSR/Moscow

 
Пример 2

Ввод

24
RENAME FakeCountry FarFarAway
ABOUT FarFarAway
DUMP
CHANGE_CAPITAL TsardomOfRussia Moscow
CHANGE_CAPITAL TsardomOfRussia Moscow
CHANGE_CAPITAL ColonialBrazil Salvador
CHANGE_CAPITAL TsardomOfRussia SaintPetersburg
RENAME TsardomOfRussia RussianEmpire
CHANGE_CAPITAL RussianEmpire Moscow
CHANGE_CAPITAL RussianEmpire SaintPetersburg
CHANGE_CAPITAL ColonialBrazil RioDeJaneiro
DUMP
RENAME ColonialBrazil EmpireOfBrazil
ABOUT RussianEmpire
RENAME EmpireOfBrazil UnitedStatesOfBrazil
CHANGE_CAPITAL RussianEmpire Petrograd
RENAME RussianEmpire RussianRepublic
RENAME RussianRepublic USSR
CHANGE_CAPITAL USSR Moscow
CHANGE_CAPITAL UnitedStatesOfBrazil Brasilia
RENAME UnitedStatesOfBrazil FederativeRepublicOfBrazil
ABOUT RussianEmpire
DUMP
RENAME USSR USSR

 

Вывод

Incorrect rename, skip
Country FarFarAway doesn't exist
There are no countries in the world
Introduce new country TsardomOfRussia with capital Moscow
Country TsardomOfRussia hasn't changed its capital
Introduce new country ColonialBrazil with capital Salvador
Country TsardomOfRussia has changed its capital from Moscow to SaintPetersburg
Country TsardomOfRussia with capital SaintPetersburg has been renamed to RussianEmpire
Country RussianEmpire has changed its capital from SaintPetersburg to Moscow
Country RussianEmpire has changed its capital from Moscow to SaintPetersburg
Country ColonialBrazil has changed its capital from Salvador to RioDeJaneiro
ColonialBrazil/RioDeJaneiro RussianEmpire/SaintPetersburg
Country ColonialBrazil with capital RioDeJaneiro has been renamed to EmpireOfBrazil
Country RussianEmpire has capital SaintPetersburg
Country EmpireOfBrazil with capital RioDeJaneiro has been renamed to UnitedStatesOfBrazil
Country RussianEmpire has changed its capital from SaintPetersburg to Petrograd
Country RussianEmpire with capital Petrograd has been renamed to RussianRepublic
Country RussianRepublic with capital Petrograd has been renamed to USSR
Country USSR has changed its capital from Petrograd to Moscow
Country UnitedStatesOfBrazil has changed its capital from RioDeJaneiro to Brasilia
Country UnitedStatesOfBrazil with capital Brasilia has been renamed to FederativeRepublicOfBrazil
Country RussianEmpire doesn't exist
FederativeRepublicOfBrazil/Brasilia USSR/Moscow
Incorrect rename, skip

 
Пример 3

Ввод

10
CHANGE_CAPITAL RussianEmpire Petrograd
RENAME USSR RussianEmpire
DUMP
ABOUT USSR
DUMP
RENAME RussianEmpire RussianEmpire
DUMP
CHANGE_CAPITAL UnitedStatesOfBrazil Brasilia
RENAME RussianEmpire UnitedStatesOfBrazil
DUMP

 

Ввод

Introduce new country RussianEmpire with capital Petrograd
Incorrect rename, skip
RussianEmpire/Petrograd
Country USSR doesn't exist
RussianEmpire/Petrograd
Incorrect rename, skip
RussianEmpire/Petrograd
Introduce new country UnitedStatesOfBrazil with capital Brasilia
Incorrect rename, skip
RussianEmpire/Petrograd UnitedStatesOfBrazil/Brasilia
*/

void ProcessOperations(int n) {
    enum class OPERATIONS {
        CHANGE_CAPITAL,
        RENAME,
        ABOUT,
        DUMP
    };

    std::map<std::string, OPERATIONS> str2op = {
        {"CHANGE_CAPITAL", OPERATIONS::CHANGE_CAPITAL},
        {"RENAME", OPERATIONS::RENAME},
        {"ABOUT", OPERATIONS::ABOUT},
        {"DUMP", OPERATIONS::DUMP}};

    std::map<std::string, std::string> country2capital;
    std::string op_as_str{};

    for (int i = 0; i < n; ++i) {
        std::cin >> op_as_str;
        switch (str2op[op_as_str]) {
            case OPERATIONS::CHANGE_CAPITAL: {
                std::string country{}, new_capital{};
                std::cin >> country;
                std::cin >> new_capital;

                if (!country2capital.count(country)) {
                    std::cout << "Introduce new country country with capital"s << new_capital << std::endl;
                    country2capital[country] = new_capital;
                } else {
                    if (country2capital[country] == new_capital) {
                        std::cout << "Country "s << country << " hasn't changed its capital"s << new_capital << std::endl;
                    } else {
                        std::cout << "Country "s << country << " has changed its capital from"s << country2capital[country] << " to "s << new_capital << std::endl;
                        country2capital[country] = new_capital;
                    }
                }
                break;
            }
            case OPERATIONS::RENAME: {
                // TODO:
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
    int n{};
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::string word1{}, word2{};
        std::cin >> word1 >> word2;
        if (BuildCharCounters(word1) == BuildCharCounters(word2)) {
            std::cout << "YES"s << std::endl;
        } else {
            std::cout << "NO"s << std::endl;
        }
    }
    return 0;
}
