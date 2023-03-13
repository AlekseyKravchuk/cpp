#include <cassert>
#include <cmath>
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

TaskStatus NextStatus(TaskStatus current_status) {
    if (current_status != TaskStatus::DONE) {
        return static_cast<TaskStatus>(static_cast<int>(current_status) + 1);
    } else {
        throw std::runtime_error("Current status is \"TaskStatus::DONE\", operation \"NextStatus\" not applicable"s);
    }
}

// overloading of prefix increment operator
TaskStatus& operator++(TaskStatus& current_status) {
    return current_status = NextStatus(current_status);
}

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = std::map<TaskStatus, int>;

template <typename T>
inline int sgn(T val) {
    return ((T{} < val) - (val < T{}));
}

void TestSGN() {
    assert(0 == sgn(0));
    assert(0 == sgn(0.0));
    assert(0 == sgn(0.0f));
    assert(1 == sgn(1.3f));
    assert(1 == sgn(110));
    assert(1 == sgn(0.0001f));
    assert(1 == sgn(0.5));
    assert(-1 == sgn(-0.0003));
    assert(-1 == sgn(-9.0f));
    assert(-1 == sgn(-234234));
    assert(-1 == sgn(-1.00003f));
}

class TeamTasks {
   public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const std::string& person) const {
        return _names2tasksinfo.at(person);
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const std::string& person) {
        ++_names2tasksinfo[person][TaskStatus::NEW];
    }

    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(const std::string& person, int task_count) {
        // auto current_status = TaskStatus::NEW;
        std::map<TaskStatus, int> updated;
        std::map<TaskStatus, int> untouched;

        TasksInfo tmp;
        for (const auto& [current_status, current_count] : _names2tasksinfo[person]) {
            int delta = task_count - current_count;
            if (delta >= 0) {
                updated[NextStatus(current_status)] += current_count;
                tmp[NextStatus(current_status)] += current_count;
            } else {
                untouched[current_status] += std::abs(delta);
                tmp[current_status] += untouched[current_status];
                updated[NextStatus(current_status)] = task_count;
                break;
            }
        }

        return {updated, untouched};
    }

   private:
    std::map<std::string, TasksInfo> _names2tasksinfo;
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

int main() {
    TeamTasks tasks;

    for (int i = 0; i < 10; ++i) {
        tasks.AddNewTask("Ivan");
    }

    std::cout << "Ivan's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

    TasksInfo updated_tasks, untouched_tasks;

    std::tie(updated_tasks, untouched_tasks) =
        tasks.PerformPersonTasks("Ivan", 4);
    std::cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    std::tie(updated_tasks, untouched_tasks) =
        tasks.PerformPersonTasks("Ivan", 3);
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

    return 0;
}