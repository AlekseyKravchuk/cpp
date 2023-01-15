#include <fstream>
#include <iostream>
#include <string>

int main() {
    using namespace std::literals;

    std::string fileToReadFrom = "hello.txt"s;
    // std::string fileToReadFrom = "helol.txt"s; // имя файла записано с ошибкой (преднамеренно)

    // объявляем экземпляр класса "ifstream", т.к. мы будем ЧИТАТЬ ИЗ файла
    std::ifstream input(fileToReadFrom);

    if (input/* .is_open() */) {
        // создадим объект типа "std::string" (строковую переменную), в который мы будем писать результат чтения из файла
        std::string line;

        // "std::getline" возвращает ссылку на поток, из которого она читает данные
        // а поток мы можем привести к типу "bool", которое будет "false", если поток закончился
        // Все потоки можно приводить к типу "bool" - они вернут "true", если с ними можно работать в данный момент.
        // когда мы считываем из файла в строковую переменную, "std::getline" считывает данные до ОПРЕДЕЛЕННОГО разделителя;
        // по умолчанию "перенос строки" и этот "перенос строки" в результурующую строку "line" не попадает
        // этот код выведет все строчки из файла и ничего лишнего
        while (getline(input, line)) {
            std::cout << line << std::endl;
        }
        std::cout << "Done." << std::endl;
    } else {
        std::cout << "Opening a file \""s << fileToReadFrom << "\" for reading failed."s << std::endl;
    }

    return 0;
}
