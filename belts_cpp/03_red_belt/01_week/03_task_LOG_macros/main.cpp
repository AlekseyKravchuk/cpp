#include <ostream>
#include <sstream>
#include <string>

#include "test_runner.h"

using namespace std::literals;

/*
==================== Задание по программированию «Макрос LOG» ====================
Вам дан класс Logger и макрос LOG

class Logger {
public:
  explicit Logger(ostream& output_stream) : os(output_stream) {
  }

  void SetLogLine(bool value) { log_line = value; }
  void SetLogFile(bool value) { log_file = value; }

  void Log(const string& message);

private:
  ostream& os;
  bool log_line = false;
  bool log_file = false;
};

#define LOG(logger, message) ...


Параметры макроса LOG — объект класса Logger и строковое сообщение, которое должно быть передано в метод Log.
В зависимости от настроек объекта класса Logger логируемое сообщение должно предваряться именем файла или номером строки.
Смотрите юнит-тесты в заготовке решения для более подробного описания поведения.

Реализуйте макрос LOG так, чтобы он добавлял в логируемое сообщение имя файла и номер строки в зависимости от настроек объекта logger.
Пришлите на проверку cpp-файл, содержащий
    - реализацию макроса LOG
    - объявление класса Logger, при этом вы можете при необходимости добавлять в него дополнительные поля и методы
    - определения всех методов класса Logger
    - log.cpp

Замечание

Если вы используете Visual Studio, у вас могут не проходить юнит-тесты из заготовки,
потому что в этой среде макрос __FILE__ раскрывается в абсолютный путь до файла. Учитывайте это при отладке своего решения.
*/

class Logger {
   public:
    explicit Logger(std::ostream& output_stream) : os(output_stream) {}

    void SetLogLine(bool value) { log_line = value; }
    void SetLogFile(bool value) { log_file = value; }

    void Log(const std::string& message) {
        os << message;
    }

    bool line() { return log_line; }
    bool file() { return log_file; }

   private:
    std::ostream& os;
    bool log_line = false;
    bool log_file = false;
};

#define LOG(logger, message) {                          \
        std::ostringstream oss;                         \
        if (logger.file() && logger.line()) {           \
            oss << __FILE__ << ":"s << __LINE__ << ' '; \
        } else if (logger.file() && !logger.line()) {   \
            oss << __FILE__ << ' ';                     \
        } else if (!logger.file() && logger.line()) {   \
            oss << "Line "s << __LINE__ << ' ';         \
        }                                               \
        oss << message << "\n"s;                        \
        logger.Log(oss.str());                          \
}                                                       

void TestLog() {
/* Для написания юнит-тестов в этой задаче нам нужно фиксировать конкретные номера строк в ожидаемом значении
 * Если мы добавляем какой-то код выше функции TestLog, то эти номера строк меняются, и наш тест начинает падать. Это неудобно.
 *
 * Чтобы этого избежать, мы используем специальный макрос #line
 * (http://en.cppreference.com/w/cpp/preprocessor/line), который позволяет
 * переопределить номер строки, а также имя файла. Благодаря ему, номера
 * строк внутри функции TestLog будут фиксированы независимо от того, какой
 * код мы добавляем перед ней*/
#line 1 "logger.cpp"

    std::ostringstream logs;
    Logger l(logs);
    LOG(l, "hello");

    l.SetLogFile(true);
    LOG(l, "hello");

    l.SetLogLine(true);
    LOG(l, "hello");

    l.SetLogFile(false);
    LOG(l, "hello");

    std::string expected = "hello\n";
    expected += "logger.cpp hello\n";
    expected += "logger.cpp:10 hello\n";
    expected += "Line 13 hello\n";
    ASSERT_EQUAL(logs.str(), expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestLog);
}
