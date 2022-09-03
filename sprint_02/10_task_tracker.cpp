#include <iostream>
#include <map>
#include <numeric>  // for std::accumulate() and std::reduce()
#include <string>
#include <tuple>
#include <utility>  // for std::pair
#include <vector>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
    NEW,          // новая
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};

// TasksInfo - это словарь, хранящий количество задач по статусам, {TaskStatus: Num_of_tasks_in_that_status}.
// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
   private:
    map<string, map<TaskStatus, int> > _task_mgr;

   public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const string& person) const;

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string& person) {
        ++_task_mgr[person][TaskStatus::NEW];
    }

    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
        // if person with name "person" doesn't exist
        if (_task_mgr.count(person) == 0) {
            return {TasksInfo(), TasksInfo()};
        }

        int real_task_count = std::accumulate(_task_mgr[person].begin(), _task_mgr[person].end(), 0,
                                              [](int value, const std::map<TaskStatus, int>::value_type& p) {
                                                  return value + p.second;
                                              });

        if (task_count > real_task_count) {
            task_count = real_task_count;
        }

        TasksInfo tasks_per_status = _task_mgr[person];  // temporary holder for "_task_mgr" map
        TasksInfo changes;

        for (const auto [status, N_tasks] : _task_mgr[person]) {
            if (task_count <= 0) {
                break;
            }

            switch (status) {
                case TaskStatus::NEW:
                case TaskStatus::IN_PROGRESS:
                case TaskStatus::TESTING:
                    if (N_tasks < task_count) {
                        task_count -= N_tasks;
                        tasks_per_status[status] = 0;
                        tasks_per_status[static_cast<TaskStatus>(static_cast<int>(status) + 1)] += N_tasks;
                    }
                    break;
                case TaskStatus::DONE:
                    //
                    break;
                default:
                    cout << "Unknown request"s << endl;
            }
        }

        while (task_count > 0) {
            // TODO
        }

        return {TasksInfo(), TasksInfo()};
    }
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь.
void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks"s
         << ", "s << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress"s
         << ", "s << tasks_info[TaskStatus::TESTING] << " tasks are being tested"s
         << ", "s << tasks_info[TaskStatus::DONE] << " tasks are done"s << endl;
}

int main() {
    TeamTasks tasks;
    // tasks.AddNewTask("Ilia"s);

    // for (int i = 0; i < 3; ++i) {
    //     tasks.AddNewTask("Ivan"s);
    // }

    // cout << "Ilia's tasks: "s;
    // PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"s));
    // cout << "Ivan's tasks: "s;
    // PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));

    // TasksInfo updated_tasks, untouched_tasks;

    // tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 2);
    // cout << "Updated Ivan's tasks: "s;
    // PrintTasksInfo(updated_tasks);
    // cout << "Untouched Ivan's tasks: "s;
    // PrintTasksInfo(untouched_tasks);

    // tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 2);
    // cout << "Updated Ivan's tasks: "s;
    // PrintTasksInfo(updated_tasks);
    // cout << "Untouched Ivan's tasks: "s;
    // PrintTasksInfo(untouched_tasks);
}