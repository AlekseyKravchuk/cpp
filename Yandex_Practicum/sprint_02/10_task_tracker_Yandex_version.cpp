#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std::literals;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
    NEW,          // новая
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = std::map<TaskStatus, int>;

// Выделим в отдельную функцию получение следующего по приоритету типа задачи
// Функция налагает требование на входной параметр: он не должен быть равен DONE
// При этом task_status явно не сравнивается с DONE, что позволяет
// сделать эту функцию максимально эффективной
TaskStatus Next(TaskStatus task_status) {
    return static_cast<TaskStatus>(static_cast<int>(task_status) + 1);
}

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = std::map<TaskStatus, int>;

class TeamTasks {
   public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const std::string& person) const;

    // Добавить новую задачу (в статусе NEW) для конкретного разработчика
    void AddNewTask(const std::string& person);

    // Обновить статусы по данному количеству задач конкретного разработчика
    std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(
        const std::string& person, int task_count);

   private:
    std::map<std::string, TasksInfo> _person_tasks;
};

const TasksInfo& TeamTasks::GetPersonTasksInfo(const std::string& person) const {
    return _person_tasks.at(person);
}

void TeamTasks::AddNewTask(const std::string& person) {
    ++_person_tasks[person][TaskStatus::NEW];
}

// Функция для удаления нулей из словаря
void RemoveZeros(TasksInfo& tasks_info) {
    // Соберём те статусы, которые нужно убрать из словаря
    std::vector<TaskStatus> statuses_to_remove;
    for (const auto& task_item : tasks_info) {
        if (task_item.second == 0) {
            statuses_to_remove.push_back(task_item.first);
        }
    }

    for (const TaskStatus status : statuses_to_remove) {
        tasks_info.erase(status);
    }
}

std::tuple<TasksInfo, TasksInfo> TeamTasks::PerformPersonTasks(
    const std::string& person, int task_count) {
    TasksInfo updated_tasks, untouched_tasks;

    // Здесь и далее мы будем пользоваться тем фактом, что в std::map оператор []
    // в случае отсутствия ключа инициализирует значение по умолчанию, если это возможно.
    TasksInfo& tasks = _person_tasks[person];

    // Посчитаем, сколько задач каждого из статусов нужно обновить, пользуясь тем фактом,
    // что по умолчанию enum class инциализирует значения от нуля по возрастанию.
    for (TaskStatus status = TaskStatus::NEW; status != TaskStatus::DONE; status = Next(status)) {
        // Считаем обновлённые
        updated_tasks[Next(status)] = std::min(task_count, tasks[status]);
        // Считаем, сколько осталось обновить
        task_count -= updated_tasks[Next(status)];
    }

    // Обновляем статус текущих задач в соответствии с информацией об обновлённых
    // и находим количество нетронутых
    for (TaskStatus status = TaskStatus::NEW;
         status != TaskStatus::DONE;
         status = Next(status)) {
        untouched_tasks[status] = tasks[status] - updated_tasks[Next(status)];
        tasks[status] += updated_tasks[status] - updated_tasks[Next(status)];
    }
    // По условию, DONE задачи не нужно возвращать в необновлённых задачах
    tasks[TaskStatus::DONE] += updated_tasks[TaskStatus::DONE];

    // По условию в словарях не должно быть нулей
    RemoveZeros(updated_tasks);
    RemoveZeros(untouched_tasks);
    RemoveZeros(_person_tasks.at(person));

    return {updated_tasks, untouched_tasks};
}

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

int main() {
    // Test_1_base();
    Test_2_failed_case_1_102();
    return 0;
}