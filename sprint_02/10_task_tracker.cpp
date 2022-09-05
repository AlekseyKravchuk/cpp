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
    const TasksInfo& GetPersonTasksInfo(const string& person) const {
        return _task_mgr.at(person);
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string& person) {
        ++_task_mgr[person][TaskStatus::NEW];
    }

    // Обновить статусы по данному количеству задач конкретного разработчика
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int tasksToProcess) {
        // if person with name "person" doesn't exist
        if (_task_mgr.count(person) == 0) {
            return {TasksInfo(), TasksInfo()};
        }

        int availableTasks = std::accumulate(_task_mgr[person].begin(), _task_mgr[person].end(), 0,
                                             [](int value, const std::map<TaskStatus, int>::value_type& p) {
                                                 return value + p.second;
                                             }) -
                             _task_mgr[person][TaskStatus::DONE];

        if (tasksToProcess > availableTasks) {
            tasksToProcess = availableTasks;
        }

        TasksInfo updatedTasksPerStatus = _task_mgr[person];  // начальное количество задач по каждому статусу
        TasksInfo changesPerStatus;                           // количество обновившихся задач по каждому статусу
        TasksInfo notChangedPerStatus;                        // количество задач, статус которых не DONE и не изменился (по каждому статусу)

        for (const auto [status, N_tasks] : _task_mgr[person]) {
            if (tasksToProcess <= 0) {
                break;
            }

            if (N_tasks == 0) {
                continue;
            }

            switch (status) {
                case TaskStatus::NEW:
                case TaskStatus::IN_PROGRESS:
                case TaskStatus::TESTING:
                    if (N_tasks < tasksToProcess) {
                        changesPerStatus[status] += N_tasks;
                        tasksToProcess -= N_tasks;
                        updatedTasksPerStatus[status] = 0;  // обнулить количество задач с текущим статусом

                        // перевести N_tasks в следующий статус (в порядке следования: NEW → IN_PROGRESS → TESTING → DONE)
                        updatedTasksPerStatus[static_cast<TaskStatus>(static_cast<int>(status) + 1)] += N_tasks;
                    } else {
                        changesPerStatus[status] += tasksToProcess;
                        tasksToProcess = 0;
                        updatedTasksPerStatus[status] -= tasksToProcess;
                    }
                    break;
                case TaskStatus::DONE:
                    break;
                default:
                    cout << "Unknown request"s << endl;
            }
        }

        // get statistics about all the tasks which are NOT DONE and whose status didn't change
        for (const auto& [status, N_tasks] : _task_mgr[person]) {
            if (status == TaskStatus::DONE || N_tasks == 0) {
                continue;
            }

            notChangedPerStatus[status] = N_tasks - changesPerStatus[status];
        }

        // update task manager according to processed tasks per certain status
        _task_mgr[person] = updatedTasksPerStatus;

        return {changesPerStatus, notChangedPerStatus};
    }
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks"
         << ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress"
         << ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested"
         << ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}
int main() {
    TeamTasks tasks;

    tasks.AddNewTask("Ilia");

    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan");
    }
    cout << "Ilia's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
    cout << "Ivan's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

    TasksInfo updated_tasks, untouched_tasks;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);
}


/*
Ilia's tasks: 1 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
Ivan's tasks: 3 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
BUG ==>>>> Updated Ivan's tasks: 2 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
Untouched Ivan's tasks: 1 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
Updated Ivan's tasks: 2 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
Untouched Ivan's tasks: 1 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done



Ilia's tasks: 1 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
Ivan's tasks: 3 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
Updated Ivan's tasks: 0 new tasks, 2 tasks in progress, 0 tasks are being tested, 0 tasks are done
Untouched Ivan's tasks: 1 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
Updated Ivan's tasks: 0 new tasks, 1 tasks in progress, 1 tasks are being tested, 0 tasks are done
Untouched Ivan's tasks: 0 new tasks, 1 tasks in progress, 0 tasks are being tested, 0 tasks are done 
*/