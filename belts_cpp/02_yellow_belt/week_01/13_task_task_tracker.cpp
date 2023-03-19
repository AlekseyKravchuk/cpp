#include <array>  // std::array
#include <iostream>
#include <map>
#include <stdexcept>
#include <tuple>

using namespace std::literals;

/*
Трекер задач

Реализуйте класс TeamTasks, позволяющий хранить статистику по статусам задач команды разработчиков:

// Перечислимый тип для статуса задачи
enum class TaskStatus {
  NEW,          // новая
  IN_PROGRESS,  // в разработке
  TESTING,      // на тестировании
  DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:
  // Получить статистику по статусам задач конкретного разработчика
  const TasksInfo& GetPersonTasksInfo(const string& person) const;

  // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
  void AddNewTask(const string& person);

  // Обновить статусы по данному количеству задач конкретного разработчика,
  // подробности см. ниже
  tuple<TasksInfo, TasksInfo> PerformPersonTasks(
      const string& person, int task_count);
};

Метод PerformPersonTasks должен реализовывать следующий алгоритм:
    Рассмотрим все не выполненные задачи разработчика person.
    Упорядочим их по статусам: сначала все задачи в статусе NEW, затем все задачи в статусе IN_PROGRESS и, наконец, задачи в статусе TESTING.
    Рассмотрим первые task_count задач и переведём каждую из них в следующий статус в соответствии с естественным порядком: NEW → IN_PROGRESS → TESTING → DONE.
    Вернём кортеж из двух элементов: информацию о статусах обновившихся задач (включая те, которые оказались в статусе DONE) и информацию о статусах остальных не выполненных задач.

Гарантируется, что task_count является положительным числом. Если task_count превышает текущее количество невыполненных задач разработчика, достаточно считать, что task_count равен количеству невыполненных задач: дважды обновлять статус какой-либо задачи в этом случае не нужно.

Кроме того, гарантируется, что метод GetPersonTasksInfo не будет вызван для разработчика, не имеющего задач.
Пример работы метода PerformPersonTasks

Предположим, что у конкретного разработчика имеется 10 задач со следующими статусами:

    NEW — 3

    IN_PROGRESS — 2

    TESTING — 4

    DONE — 1

Поступает команда PerformPersonTasks с параметром task_count = 4, что означает обновление статуса для 3 задач c NEW до IN_PROGRESS и для 1 задачи с IN_PROGRESS до TESTING. Таким образом, новые статусы задач будут следующими:

    IN_PROGRESS — 3 обновлённых, 1 старая

    TESTING — 1 обновлённая, 4 старых

    DONE — 1 старая

В этом случае необходимо вернуть кортеж из 2 словарей:

    Обновлённые задачи: IN_PROGRESS — 3, TESTING — 1.

    Не обновлённые задачи, исключая выполненные: IN_PROGRESS — 1, TESTING — 4.

Словари не должны содержать лишних элементов, то есть статусов, которым соответствует ноль задач.
Примечание

Обновление словаря одновременно с итерированием по нему может привести к непредсказуемым последствиям. При возникновении такой необходимости рекомендуется сначала в отдельном временном словаре собрать информацию об обновлениях, а затем применить их к основному словарю.
*/

// Перечислимый тип для статуса задачи
enum class TaskStatus {
    NEW,          // новая
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};

// дополнительный массив для возможности итерирования по статусам перечисления "TaskStatus"
// const std::array<TaskStatus, 4> all_statuses = {
//     TaskStatus::NEW,
//     TaskStatus::IN_PROGRESS,
//     TaskStatus::TESTING,
//     TaskStatus::DONE};

TaskStatus NextStatus(TaskStatus status) {
    if (status != TaskStatus::DONE) {
        return static_cast<TaskStatus>((static_cast<int>(status) + 1));
    } else {
        throw std::runtime_error("Current status is \"TaskStatus::DONE\", operation \"NextStatus\" not applicable"s);
    }
}

// overloading of prefix increment operator for "TaskStatus"
TaskStatus& operator++(TaskStatus& status) {
    return status = NextStatus(status);
}

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = std::map<TaskStatus, int>;

class TeamTasks {
   public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const std::string& person) const {
        return _names2tinfo.at(person);
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const std::string& person) {
        ++_names2tinfo[person][TaskStatus::NEW];
    }

    // Обновить статусы по данному количеству задач конкретного разработчика
    std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(const std::string& person, int task_count) {
        TasksInfo updated_tasks;
        TasksInfo untouched_tasks;
        TasksInfo& tasks = _names2tinfo[person];
        int done_tasks_count = tasks[TaskStatus::DONE];

        for (auto status = TaskStatus::NEW; status != TaskStatus::DONE; ++status) {
            if (tasks.count(status) == 0) {
                continue;
            }

            // "task_count == 0" => больше задач, подлежащих выполнению, нет и "status != TaskStatus::DONE"
            // => все задачи данного статуса должны быть учтены как необработанные, т.е. в "untouched_tasks"
            if (task_count == 0) {
                untouched_tasks[status] = tasks.at(status);
                continue;
            }

            int count = tasks.at(status);
            auto next_status = NextStatus(status);

            // проверяем, не осталось ли необработанных задач в текущем статусе
            // если "task_count < count", то в текущем статусе останутся необработанные задачи
            if (task_count < count) {
                updated_tasks[next_status] = task_count;
                untouched_tasks[status] = count - task_count;
                task_count = 0;
            } else {
                // если "task_count >= count", необработанных задач текущего статуса не останется
                updated_tasks[next_status] = count;
                task_count -= count;
            }

            // ведем отдельный учет DONE-задач (выполненных задач)
            if (next_status == TaskStatus::DONE) {
                if (updated_tasks.count(next_status)) {
                    done_tasks_count += updated_tasks[next_status];
                }
            }
        }

        // обнуляем словарь "tasks"
        tasks.clear();

        // добавляем DONE-задачи
        if (done_tasks_count) {
            tasks[TaskStatus::DONE] = done_tasks_count;
        }

        // восстанавливаем актуальные значения "_names2tinfo" в соответствии с "updated_tasks" и "untouched_tasks"
        for (auto status = TaskStatus::NEW; status != TaskStatus::DONE; ++status) {
            if (untouched_tasks.count(status)) {
                tasks[status] += untouched_tasks[status];
            }

            if (updated_tasks.count(status)) {
                tasks[status] += updated_tasks[status];
            }
        }

        return {updated_tasks, untouched_tasks};
    }

   private:
    std::map<std::string, TasksInfo> _names2tinfo;
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
    std::cout << tasks_info[TaskStatus::NEW] << " new tasks"
              << ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress"
              << ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested"
              << ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << std::endl;
}

void Test_1_base() {
    TeamTasks tasks;

    tasks.AddNewTask("Ilia");
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan");
    }

    std::cout << "Ilia's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
    std::cout << "Ivan's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

    TasksInfo updated_tasks, untouched_tasks;

    std::tie(updated_tasks, untouched_tasks) =
        tasks.PerformPersonTasks("Ivan", 2);
    std::cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    std::tie(updated_tasks, untouched_tasks) =
        tasks.PerformPersonTasks("Ivan", 2);
    std::cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);
}

void Test_2_failed_case_1_102() {
    TeamTasks tasks;
    TasksInfo updated_tasks, untouched_tasks;

    for (int i = 0; i < 5; ++i) {
        tasks.AddNewTask("Alice"s);
    }

    std::tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice"s, 5);
    std::cout << "Updated Alice's tasks: ";
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Alice's tasks: ";
    PrintTasksInfo(untouched_tasks);
    PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
    std::cout << "======================================================="s << std::endl;

    std::tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice"s, 5);
    std::cout << "Updated Alice's tasks: ";
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Alice's tasks: ";
    PrintTasksInfo(untouched_tasks);
    PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
    std::cout << "======================================================="s << std::endl;

    std::tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice"s, 1);
    std::cout << "Updated Alice's tasks: ";
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Alice's tasks: ";
    PrintTasksInfo(untouched_tasks);
    PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
    std::cout << "======================================================="s << std::endl;

    for (int i = 0; i < 5; ++i) {
        tasks.AddNewTask("Alice"s);
    }

    std::tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice"s, 2);
    std::cout << "Updated Alice's tasks: ";
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Alice's tasks: ";
    PrintTasksInfo(untouched_tasks);
    PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
    std::cout << "======================================================="s << std::endl;

    std::tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice"s, 4);
    std::cout << "Updated Alice's tasks: ";
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Alice's tasks: ";
    PrintTasksInfo(untouched_tasks);
    PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
    std::cout << "======================================================="s << std::endl;
}

void Test_3() {
    TeamTasks tasks;
    TasksInfo updated_tasks;
    TasksInfo untouched_tasks;

    for (auto i = 0; i < 5; ++i) {
        tasks.AddNewTask("Lisa");
    }

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa"s, 5);
    std::cout << "Updated Lisa's tasks: "s;
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Lisa's tasks: "s;
    PrintTasksInfo(untouched_tasks);  // [{"IN_PROGRESS": 5}, {}]
    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"s));
    std::cout << "======================================================="s << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa"s, 5);
    std::cout << "Updated Lisa's tasks: "s;
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Lisa's tasks: "s;
    PrintTasksInfo(untouched_tasks);  // [{"TESTING": 5}, {}]
    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"s));
    std::cout << "======================================================="s << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa"s, 1);
    std::cout << "Updated Lisa's tasks: "s;
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Lisa's tasks: "s;
    PrintTasksInfo(untouched_tasks);  // [{"DONE": 1}, {"TESTING": 4}]
    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"s));
    std::cout << "======================================================="s << std::endl;

    for (auto i = 0; i < 5; ++i) {
        tasks.AddNewTask("Lisa"s);
    }

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa"s, 2);
    std::cout << "Updated Lisa's tasks: "s;
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Lisa's tasks: "s;
    PrintTasksInfo(untouched_tasks);                    // [{"IN_PROGRESS": 2}, {"NEW": 3, "TESTING": 4}]
    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"s));  // ==> {"NEW": 3, "IN_PROGRESS": 2, "TESTING": 4, "DONE": 1}
    std::cout << "======================================================="s << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa"s, 4);
    std::cout << "Updated Lisa's tasks: "s;
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Lisa's tasks: "s;
    PrintTasksInfo(untouched_tasks);                    // [{"IN_PROGRESS": 3, "TESTING": 1}, {"IN_PROGRESS": 1, "TESTING": 4}]
    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"s));  // {"IN_PROGRESS": 4, "TESTING": 5, "DONE": 1} <<<<< !!!!!
    std::cout << "======================================================="s << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa"s, 5);
    std::cout << "Updated Lisa's tasks: "s;
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Lisa's tasks: "s;
    PrintTasksInfo(untouched_tasks);                    // [{"TESTING": 4, "DONE": 1}, {"TESTING": 4}]
    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"s));  // {"TESTING": 8, "DONE": 2}
    std::cout << "======================================================="s << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa"s, 10);
    std::cout << "Updated Lisa's tasks: "s;
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Lisa's tasks: "s;
    PrintTasksInfo(untouched_tasks);                    // [{"DONE": 8}, {}]
    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"s));  // {"DONE": 10}
    std::cout << "======================================================="s << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa"s, 10);
    std::cout << "Updated Lisa's tasks: "s;
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Lisa's tasks: "s;
    PrintTasksInfo(untouched_tasks);                    // [{}, {}]
    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"s));  // {"DONE": 10}
    std::cout << "======================================================="s << std::endl;

    tasks.AddNewTask("Lisa"s);

    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"s));  // {"NEW": 1, "DONE": 10}
    std::cout << "======================================================="s << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa"s, 2);
    std::cout << "Updated Lisa's tasks: "s;
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Lisa's tasks: "s;
    PrintTasksInfo(untouched_tasks);                    // [{"IN_PROGRESS": 1}, {}]
    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"s));  // {"IN_PROGRESS": 1, "DONE": 10}
    std::cout << "======================================================="s << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Bob", 3);
    std::cout << "Updated Bob's tasks: "s;
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Bob's tasks: "s;
    PrintTasksInfo(untouched_tasks);  // [{}, {}]
    PrintTasksInfo(tasks.GetPersonTasksInfo("Bob"s));
    std::cout << "======================================================="s << std::endl;
}

int main() {
    Test_2_failed_case_1_102();

    return 0;
}