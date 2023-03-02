#include <fstream>
#include <iostream>
// #include <istream>  // std::istream
#include <map>
#include <numeric>  // std::gcd
#include <set>
#include <sstream>
#include <stdexcept>  // std::domain_error
#include <string>
#include <vector>

using namespace std::literals;

// #define _GLIBCXX_DEBUG 1  // включить режим отладки

/*
Задание по программированию
«Работа с сервером времени»



Вам дана функция string AskTimeServer(), про которую известно следующее:
    - в процессе своей работы она обращается по сети к удалённому серверу, запрашивая текущее время;
    - если обращение к серверу прошло успешно, функция возвращает текущее время в виде строки;
    - если в процессе обращения к серверу возникла сетевая проблема, функция выбрасывает исключение system_error;
    - функция может выбрасывать другие исключения, чтобы сообщить о других проблемах.

Используя функцию AskTimeServer, напишите класс TimeServer со следующим интерфейсом:

class TimeServer {
public:
  string GetCurrentTime();
private:
  string LastFetchedTime = "00:00:00";
};



Метод GetCurrentTime должен вести себя так:

    - он должен вызвать функцию AskTimeServer, записать её результат в поле LastFetchedTime и вернуть значение этого поля;
    - если AskTimeServer выбросила исключение system_error, метод GetCurrentTime должен его поймать и вернуть текущее значение поля LastFetchedTime.
      Таким образом мы скрываем от пользователя сетевые проблемы, возвращая значение, которое было получено при последнем успешном обращении к серверу;
    - если AskTimeServer выбросила другое исключение, метод GetCurrentTime должен пробросить его дальше, потому что в рамках класса TimeServer мы не знаем, как обрабатывать проблемы, не связанные со сбоями сети.


Как выполнять задание

Вам дан файл, содержащий заготовку класса TimeServer.
В нём вам надо реализовать метод GetCurrentTime так, как описано выше.
Файл содержит пустую функцию AskTimeServer.
Для тестирования своей реализации вы можете пробовать добавлять разные команды в её тело:
    - возврат строки;
    - выброс исключения system_error (класс system_error принимает в конструкторе параметр типа error_code,
      поэтому самый простой способ выбросить это исключение — throw system_error(error_code());
    - выброс других исключений.
*/

std::string AskTimeServer() {
    /* Для тестирования повставляйте сюда код, реализующий различное поведение этой функии:
     * нормальный возврат строкового значения
     * выброс исключения system_error
     * выброс другого исключения с сообщением.
     */
    // return "02:35:40"s;
    // throw std::system_error();
    throw;
}

class TimeServer {
   public:
    std::string GetCurrentTime() {
        /* Реализуйте этот метод:
            * если AskTimeServer() вернула значение, запишите его в last_fetched_time и верните
            * если AskTimeServer() бросила исключение system_error, верните текущее значение
            поля last_fetched_time
            * если AskTimeServer() бросила другое исключение, пробросьте его дальше.
        */
        try {
            last_fetched_time = AskTimeServer();
        } catch (const std::system_error& sys_error) {
            return last_fetched_time;
        } catch (...) {
            throw;
        }

        return last_fetched_time;
    }

   private:
    std::string last_fetched_time = "00:00:00";
};

int main() {
    // Меняя реализацию функции AskTimeServer, убедитесь, что это код работает корректно
    TimeServer ts;
    try {
        std::cout << ts.GetCurrentTime() << std::endl;
    } catch (std::exception& e) {
        std::cout << "Exception got: " << e.what() << std::endl;
    }
    return 0;
}
