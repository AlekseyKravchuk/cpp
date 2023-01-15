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
    int getRealNumOfUnprocessedTasks(const string& person) {
        int totalNumTasks = std::accumulate(_task_mgr[person].begin(), _task_mgr[person].end(), 0,
                                            [](int value, const std::map<TaskStatus, int>::value_type& p) {
                                                return value + p.second;
                                            });
        int performedTasksNum = _task_mgr.at(person).count(TaskStatus::DONE) ? _task_mgr.at(person).at(TaskStatus::DONE) : 0;
        return totalNumTasks - performedTasksNum;
    }

    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const string& person) const {
        return _task_mgr.at(person);
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string& person) {
        ++_task_mgr[person][TaskStatus::NEW];
    }

    // Обновить статусы по данному количеству задач конкретного разработчика
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int taskCntr) {
        // if person with name "person" doesn't exist
        if (_task_mgr.count(person) == 0) {
            return {TasksInfo(), TasksInfo()};
        }

        int realNumOfUnprocessedTasks = getRealNumOfUnprocessedTasks(person);
        taskCntr = (taskCntr <= realNumOfUnprocessedTasks) ? taskCntr : realNumOfUnprocessedTasks;

        TasksInfo updated;                  // статистика по обновившимся задачам по каждому статусу
        TasksInfo untouched;                // статистика по задачам, статус которых не DONE и не изменился (по каждому статусу)
        TasksInfo tmp = _task_mgr[person];  // временное хранилище мапы для текущего разраба, здесь фиксируются все изменения

        for (const auto [status, N] : _task_mgr[person]) {
            if (status == TaskStatus::DONE) {
                break;
            }

            TaskStatus nextStatus = static_cast<TaskStatus>(static_cast<int>(status) + 1);

            switch (status) {
                case TaskStatus::NEW:
                case TaskStatus::IN_PROGRESS:
                case TaskStatus::TESTING:
                    if (N <= taskCntr) {
                        // все задачи из текущего статуса были переведены в следующий статус
                        // => невыполненных задач с этим статусом нет => старых задач не добавилось
                        updated[nextStatus] += N;

                        // но, возможно, ещё остались задачи, которые должны быть выполнены
                        // если taskCntr обнулится, то на этом цикл закончится
                        taskCntr -= N;

                        // вносим соответствующие изменения в хранилище изменений
                        tmp[status] -= N;
                        tmp[nextStatus] += N;

                    } else {  // N > taskCntr, в том числе когда taskCntr уже равен 0
                        if (taskCntr == 0) {
                            untouched[status] = N;
                        } else {  // taskCntr != 0
                            // из текущего статуса НЕ все задачи перекочевали в следующий статус
                            updated[nextStatus] += taskCntr;

                            // => нужно учесть те задачи, которые остались необработанными
                            int n_untouched = N - taskCntr;
                            untouched[status] = n_untouched;

                            // вносим соответствующие изменения в хранилище изменений
                            tmp[status] -= taskCntr;
                            tmp[nextStatus] += taskCntr;

                            // зануляем taskCntr, поскольку обработаны все задачи, поступившие на вход
                            taskCntr = 0;
                        }
                    }
                    break;
                default:
                    cout << "Unknown request"s << endl;
            }
        }

        // обновляем _task_mgr[person]/ lj, добавляем из tmp только ключи с ненулевыми значениями
        _task_mgr[person].clear();
        for (auto [status, N_tasks] : tmp) {
            if (N_tasks) {
                _task_mgr[person].insert({status, N_tasks});
            }
        }

        return {updated, untouched};
    }
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
/* void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks"
         << ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress"
         << ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested"
         << ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
} */

void PrintTasksInfo(const TasksInfo& tasks_info) {
    if (tasks_info.count(TaskStatus::NEW)) {
        std::cout << "NEW: " << tasks_info.at(TaskStatus::NEW) << " ";
    }
    if (tasks_info.count(TaskStatus::IN_PROGRESS)) {
        std::cout << "IN_PROGRESS: " << tasks_info.at(TaskStatus::IN_PROGRESS) << " ";
    }
    if (tasks_info.count(TaskStatus::TESTING)) {
        std::cout << "TESTING: " << tasks_info.at(TaskStatus::TESTING) << " ";
    }
    if (tasks_info.count(TaskStatus::DONE)) {
        std::cout << "DONE: " << tasks_info.at(TaskStatus::DONE) << " ";
    }
}

void PrintTasksInfo(const TasksInfo& updated_tasks, const TasksInfo& untouched_tasks) {
    std::cout << "Updated: [";
    PrintTasksInfo(updated_tasks);
    std::cout << "] ";

    std::cout << "Untouched: [";
    PrintTasksInfo(untouched_tasks);
    std::cout << "] ";

    std::cout << std::endl;
}

int main() {
    TeamTasks tasks;
    TasksInfo updated_tasks;
    TasksInfo untouched_tasks;

    /* ========================= TEST 3 =========================*/
    std::cout << "\nLisa" << std::endl;

    for (auto i = 0; i < 5; ++i) {
        tasks.AddNewTask("Lisa");
    }

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 5);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 5}, {}]

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 5);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"TESTING": 5}, {}]

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 1);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"DONE": 1}, {"TESTING": 4}]

    for (auto i = 0; i < 5; ++i) {
        tasks.AddNewTask("Lisa");
    }

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 2);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 2}, {"NEW": 3, "TESTING": 4}]

    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"NEW": 3, "IN_PROGRESS": 2, "TESTING": 4, "DONE": 1}
    std::cout << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 4);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 3, "TESTING": 1}, {"IN_PROGRESS": 1, "TESTING": 4}]

    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"IN_PROGRESS": 4, "TESTING": 5, "DONE": 1} <<<<< !!!!!
    std::cout << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 5);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"TESTING": 4, "DONE": 1}, {"TESTING": 4}]

    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"TESTING": 8, "DONE": 2}
    std::cout << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 10);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"DONE": 8}, {}]

    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"DONE": 10}
    std::cout << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 10);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{}, {}]

    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"DONE": 10}
    std::cout << std::endl;

    tasks.AddNewTask("Lisa");

    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"NEW": 1, "DONE": 10}
    std::cout << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 2);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 1}, {}]

    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"IN_PROGRESS": 1, "DONE": 10}
    std::cout << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Bob", 3);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{}, {}]

    return 0;
}

// ========= CORRECT ANSWER for TEST 3 =========
/* Lisa
Updated: [IN_PROGRESS: 5 ] Untouched: [] 
Updated: [TESTING: 5 ] Untouched: [] 
Updated: [DONE: 1 ] Untouched: [TESTING: 4 ] 
Updated: [IN_PROGRESS: 2 ] Untouched: [NEW: 3 TESTING: 4 ] 
NEW: 3 IN_PROGRESS: 2 TESTING: 4 DONE: 1 
Updated: [IN_PROGRESS: 3 TESTING: 1 ] Untouched: [IN_PROGRESS: 1 TESTING: 4 ] 
IN_PROGRESS: 4 TESTING: 5 DONE: 1 
Updated: [TESTING: 4 DONE: 1 ] Untouched: [TESTING: 4 ] 
TESTING: 8 DONE: 2 
Updated: [DONE: 8 ] Untouched: [] 
DONE: 10 
Updated: [] Untouched: [] 
DONE: 10 
NEW: 1 DONE: 10 
Updated: [IN_PROGRESS: 1 ] Untouched: [] 
IN_PROGRESS: 1 DONE: 10 
Updated: [] Untouched: []  */